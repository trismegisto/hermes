#include "weakform/weakform.h"
#include "integrals/integrals_h1.h"
#include "boundaryconditions/boundaryconditions.h"

class WeakFormWave : public WeakForm
{
public:

  WeakFormWave(double tau, double c_squared, Solution* u_prev_sln, Solution* v_prev_sln) : WeakForm(2) {
    add_matrix_form(new MatrixFormVolWave_0_0(tau));
    add_matrix_form(new MatrixFormVolWave_0_1);
    add_matrix_form(new MatrixFormVolWave_1_0(c_squared));
    add_matrix_form(new MatrixFormVolWave_1_1(tau));

    VectorFormVolWave_0* vector_form_0 = new VectorFormVolWave_0(tau);
    vector_form_0->ext.push_back(u_prev_sln);
    add_vector_form(vector_form_0);
    
    VectorFormVolWave_1* vector_form_1 = new VectorFormVolWave_1(tau);
    vector_form_1->ext.push_back(v_prev_sln);
    add_vector_form(vector_form_1);
  };

private:
  class MatrixFormVolWave_0_0 : public WeakForm::MatrixFormVol
  {
  public:
    MatrixFormVolWave_0_0(double tau) : WeakForm::MatrixFormVol(0, 0), tau(tau) { }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      return int_u_v<Real, Scalar>(n, wt, u, v) / tau;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
    }
    
    // Member.
    double tau;
  };

  class MatrixFormVolWave_0_1 : public WeakForm::MatrixFormVol
  {
  public:
    MatrixFormVolWave_0_1() : WeakForm::MatrixFormVol(0, 1) { }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      return -int_u_v<Real, Scalar>(n, wt, u, v);
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
    }
  };

  class MatrixFormVolWave_1_0 : public WeakForm::MatrixFormVol
  {
  public:
    MatrixFormVolWave_1_0(double c_squared) : WeakForm::MatrixFormVol(1, 0), c_squared(c_squared) { }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      return c_squared * int_grad_u_grad_v<Real, Scalar>(n, wt, u, v);
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
    }
    
    // Member.
    double c_squared;
  };

  class MatrixFormVolWave_1_1 : public WeakForm::MatrixFormVol
  {
  public:
    MatrixFormVolWave_1_1(double tau) : WeakForm::MatrixFormVol(1, 1), tau(tau) { }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      return int_u_v<Real, Scalar>(n, wt, u, v) / tau;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
    }
    
    // Member.
    double tau;
  };

  class VectorFormVolWave_0 : public WeakForm::VectorFormVol
  {
  public:
    VectorFormVolWave_0(double tau) : WeakForm::VectorFormVol(0), tau(tau) { }

    template<typename Real, typename Scalar>
    Scalar vector_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      Scalar result = 0;
      Func<Scalar>* u_prev = ext->fn[0];
      for (int i = 0; i < n; i++)
        result += wt[i] * u_prev->val[i] * v->val[i];
      return result / tau;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return vector_form<scalar, scalar>(n, wt, u_ext, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return vector_form<Ord, Ord>(n, wt, u_ext, v, e, ext);
    }
    
    // Member.
    double tau;
  };

  class VectorFormVolWave_1 : public WeakForm::VectorFormVol
  {
  public:
    VectorFormVolWave_1(double tau) : WeakForm::VectorFormVol(1), tau(tau) { }

    template<typename Real, typename Scalar>
    Scalar vector_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      Scalar result = 0;
      Func<Scalar>* u_prev = ext->fn[0];
      for (int i = 0; i < n; i++)
        result += wt[i] * u_prev->val[i] * v->val[i];
      return result / tau;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return vector_form<scalar, scalar>(n, wt, u_ext, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return vector_form<Ord, Ord>(n, wt, u_ext, v, e, ext);
    }
    
    // Member.
    double tau;
  };
};