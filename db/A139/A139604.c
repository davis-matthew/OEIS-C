/*
 * A139604  A triangle of coefficients based on A139360 as an n-like set of
 *          three binomials: f(x,y,n)=ChebyshevT[n, x]*ChebyshevT[n, y] +
 *          ChebyshevT[n, x] + ChebyshevT[n, y];
 *          p(x,y,z,n)=f(x,y,n)+f(y,z,n)+f(z,x,n).
 * https://oeis.org/A139604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) is the coefficient of x^k in the polynomial * P_k(x) =
 *            4*T_k(x) + 5 * where T_k(x) is the Chebyshev polynomial of the
 *            first kind. * The overall index n enumerates the rows of
 *            coefficients of P_k(x): * row k (starting at k = 0) contains k+1
 *            terms (degrees 0..k), * and the terms are listed in...
 * Range:     i8 n <= 25, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers required for both C11 and C++17 */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* The following types are provided by the build system */
extern idx_t _floor_isqrt_u64(idx_t n);  /* unused */

/* Static storage for Chebyshev polynomials T_k(x) */
static seq_t **T = NULL;          /* array of pointers to coefficient arrays */
static idx_t T_size = 0;          /* number of T polynomials currently stored */

/* Ensure that T contains polynomials up to index r (inclusive) */
static void ensure_T(idx_t r)
{
    while (T_size <= r) {
        /* Allocate space for the (T_size)-th polynomial pointer */
        T = (seq_t **)realloc(T, sizeof(seq_t *) * (T_size + 1));

        idx_t i = T_size;  /* polynomial index to compute */

        if (i == 0) {
            /* T_0(x) = 1 */
            T[0] = (seq_t *)malloc(sizeof(seq_t));
            T[0][0] = 1;
        } else if (i == 1) {
            /* T_1(x) = x */
            T[1] = (seq_t *)malloc(sizeof(seq_t) * 2);
            T[1][0] = 0;
            T[1][1] = 1;
        } else {
            /* Compute using recurrence:
             *   T_i(x) = 2*x*T_{i-1}(x) - T_{i-2}(x)   */
            seq_t *prev  = T[i - 1];
            seq_t *prev2 = T[i - 2];
            seq_t *cur   = (seq_t *)malloc(sizeof(seq_t) * (i + 1));

            /* Initialize to zero */
            for (size_t j = 0; j < (size_t)(i + 1); ++j) cur[j] = 0;

            /* 2*x * prev  -> shift by 1 and multiply by 2 */
            for (size_t j = 0; j < (size_t)i; ++j)
                cur[j + 1] += (seq_t)2 * prev[j];

            /* Subtract prev2 */
            for (size_t j = 0; j < (size_t)(i - 1); ++j)
                cur[j] -= prev2[j];

            T[i] = cur;
        }

        ++T_size;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Sequence is 1-indexed; n >= 1 always in valid calls. */
        return 0;
    }

    /* Determine which row (k) and which position (pos) within that row
     * correspond to the overall index n.  Rows are numbered from 0. */
    double val = sqrt(8.0 * (double)(n - 1) + 1.0);
    idx_t k = (idx_t)((val - 1.0) / 2.0);

    /* Correct any rounding error */
    while ((k + 1) * (k + 2) / 2 < n) ++k;
    while (k * (k + 1) / 2 >= n) --k;

    idx_t pos = n - k * (k + 1) / 2 - 1;  /* 0-based position in row */

    /* Ensure that we have T_k(x) computed */
    ensure_T(k);

    /* Compute the desired coefficient:
     *   a(n) = 4 * T_k[pos] + (pos==0 ? 5 : 0) */
    seq_t result = (seq_t)4 * T[k][pos];
    if (pos == (idx_t)0)
        result += 5;

    return result;
}
