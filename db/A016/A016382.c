/*
 * A016382  322nd cyclotomic polynomial.
 * https://oeis.org/A016382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 132, i16 n <= 132, i32 n <= 132, i64 n <= 132,
 *            i128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A016382 – coefficients of the 322nd cyclotomic polynomial Φ₂₁₂(x)
    The 322nd cyclotomic polynomial has degree φ(322)=132.
    Its coefficients are in {−1,0,1}.  This function returns the
    coefficient of x^n in Φ₃₂₂(x).  The polynomial is computed once
    by the recursive definition
            Φ_n(x) = (xⁿ−1) / ∏_{d|n, d<n} Φ_d(x)
    and stored in a static cache.  */
    
#include <stddef.h>   /* size_t */

#define MAX_N 322               /* largest exponent appearing in Φ₃₂₂ */

/* divisors of 322 in increasing order */
static const idx_t divisors[8] = {1ULL, 2ULL, 7ULL, 14ULL, 23ULL, 46ULL, 161ULL, 322ULL};

/* multiply polynomials a (deg_a) and b (deg_b); store result in out.
   All arrays must have length at least MAX_N+1.   */
static int poly_mul(const seq_t *a, int deg_a,
                    const seq_t *b, int deg_b,
                    seq_t *out)
{
    int i, j;
    int out_deg = deg_a + deg_b;
    for (i = 0; i <= out_deg; ++i)
        out[i] = 0;
    for (i = 0; i <= deg_a; ++i) {
        for (j = 0; j <= deg_b; ++j) {
            out[i + j] += a[i] * b[j];
        }
    }
    return out_deg;
}

/* Divide polynomial num (deg_num) by den (deg_den); result stored in res.
   It is assumed that den divides num exactly.  */
static void poly_div(const seq_t *num, int deg_num,
                     const seq_t *den, int deg_den,
                     seq_t *res, int *deg_res)
{
    seq_t temp[MAX_N + 1];
    int i, j;
    for (i = 0; i <= deg_num; ++i)
        temp[i] = num[i];
    int res_deg = deg_num - deg_den;
    for (i = deg_num; i >= deg_den; --i) {
        seq_t coeff = temp[i];
        res[i - deg_den] = coeff;
        if (coeff) {
            for (j = 0; j <= deg_den; ++j)
                temp[i - deg_den + j] -= coeff * den[j];
        }
    }
    *deg_res = res_deg;
    for (i = res_deg + 1; i <= MAX_N; ++i)
        res[i] = 0;
}

seq_t a(idx_t n)
{
    /* static cache of Φ_d polynomials */
    static seq_t polys[8][MAX_N + 1];
    static int deg[8];
    static int initialized = 0;

    if (!initialized) {
        int k, i, j;
        int num_deg, den_deg, res_deg;
        seq_t num[MAX_N + 1];
        seq_t den[MAX_N + 1];
        seq_t temp[MAX_N + 1];
        seq_t res[MAX_N + 1];

        for (k = 0; k < (int)sizeof(divisors)/sizeof(divisors[0]); ++k) {
            idx_t d = divisors[k];

            /* Construct numerator x^d - 1 */
            for (i = 0; i <= (int)d; ++i)
                num[i] = 0;
            num[0] = (seq_t)(-1);
            num[(int)d] = (seq_t)1;
            num_deg = (int)d;

            /* Build denominator as product of Φ_e for all e|d and e<d */
            for (i = 0; i <= MAX_N; ++i)
                den[i] = 0;
            den[0] = (seq_t)1;
            den_deg = 0;

            for (j = 0; j < k; ++j) {
                if (d % divisors[j] != 0)
                    continue;                /* skip non‑divisors */
                /* multiply current denominator by Φ_{divisors[j]} */
                int new_deg = poly_mul(den, den_deg,
                                       polys[j], deg[j],
                                       temp);
                den_deg = new_deg;
                for (i = 0; i <= den_deg; ++i)
                    den[i] = temp[i];
            }

            /* Divide to obtain Φ_d */
            poly_div(num, num_deg, den, den_deg, res, &res_deg);

            /* store result */
            for (i = 0; i <= res_deg; ++i)
                polys[k][i] = res[i];
            for (i = res_deg + 1; i <= MAX_N; ++i)
                polys[k][i] = 0;
            deg[k] = res_deg;
        }
        initialized = 1;
    }

    /* Φ₃₂₂ corresponds to the last divisor in the list */
    if (n > (idx_t)deg[7])
        return (seq_t)0;
    return polys[7][(int)n];
}
