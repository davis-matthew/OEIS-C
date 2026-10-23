/*
 * A015736  Inverse of 1727th cyclotomic polynomial.
 * https://oeis.org/A015736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series of
 *            1/Φ_1727(x) The sequence is periodic with period 1727. The
 *            coefficients are obtained by inverting the cyclotomic polynomial
 *            Φ_1727(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ---------- local helpers --------------------------------------- */

static void compute_poly_coeff(void);

/* --------------------------------------------------------------- */

static /*coefficients of Φ_1727(x)*/
seq_t poly_coeff[1561];
static bool poly_computed = false;

static /*first 1727 terms of the sequence*/
seq_t period[1727];
static bool period_computed = false;

/* --------------------------------------------------------------- */
/* main entry function                                                  */
seq_t a(idx_t n)
{
    if (!period_computed)
    {
        /* build polynomial coefficients once */
        if (!poly_computed)
            compute_poly_coeff();
        /* compute first 1727 terms using the linear recurrence
           derived from Φ_1727(x) * S(x) = 1   (formal power series)  */
        period[0] = 1;
        const int d = 1560;
        const int perlen = 1727;
        for (int i = 1; i < perlen; ++i)
        {
            seq_t sum = 0;
            int max_k = (i < d ? i : d);
            for (int k = 1; k <= max_k; ++k)
                sum += poly_coeff[k] * period[i - k];
            period[i] = -sum;
        }
        period_computed = true;
    }
    return period[(size_t)(n % 1727)];
}

/* --------------------------------------------------------------- */
/* compute the coefficients of Φ_1727(x) on the fly                */
static void compute_poly_coeff(void)
{
    const int p = 11;
    const int q = 157;              /* 1727 = p * q          */
    const int deg_f = p - 1;        /* degree of Φ_p(x)       */
    const int deg_g = (p - 1) * q;  /* degree of x^{(p-1)q}  */

    /* g(x) = Σ_{k=0}^{p-1} x^{k*q}  */
    /* Use 64‑bit ints for temporary values – the coefficients stay
       small (0 or ±1).  */
    static long long g[1571];       /* size = deg_g + 1 = 1571 */
    for (int i = 0; i <= deg_g; ++i)
        g[i] = 0;
    for (int k = 0; k < p; ++k)
        g[k * q] = 1;

    /* long division:  g(x)  ÷  f(x),  f(x) = 1 + x + … + x^{p-1}  */
    for (int i = deg_g; i >= deg_f; --i)
    {
        long long c = g[i];
        if (c != 0)
        {
            poly_coeff[i - deg_f] = (seq_t)c;   /* coefficient of x^{i-deg_f} */
            /* subtract c * f(x) shifted by i-deg_f   */
            for (int j = 0; j <= deg_f; ++j)
                g[i - j] -= c;
        }
    }
    /* poly_coeff[0] is the constant term (1).  All remaining coeffs
       are in {-1,0,1}.  */
    poly_computed = true;
}
