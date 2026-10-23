/*
 * A015145  Triangle of (Gaussian) q-binomial coefficients for q=-20.
 * https://oeis.org/A015145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the (n‑th) entry of the triangle of Gaussian q‑binomial
 *            coefficients for q = –20, listed row by row starting with
 *            C(0,0)=1
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 22, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   _floor_isqrt_u64, gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_int.h"
#include "../utils/gcd.h"
#include "../utils/_floor_isqrt_u64.h"

static seq_t gaussian_q_binomial(idx_t n, idx_t k)
{
    /*  Compute the Gaussian q‑binomial coefficient C_q(n,k) with q = –20. */
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n - k)
        k = n - k;                 /* use symmetry to minimise the loop */

    idx_t r = k;
    /* Allocate temporary arrays for numerators and denominators.  */
    seq_t *num = (seq_t *)malloc(r * sizeof(seq_t));
    seq_t *den = (seq_t *)malloc(r * sizeof(seq_t));
    seq_t q = (seq_t)-20;

    for (idx_t i = 0; i < r; ++i) {
        num[i] = pow_int(q, n - r + i + 1) - (seq_t)1;
        den[i] = pow_int(q, i + 1) - (seq_t)1;
    }

    /* Cancel common factors between any numerator and denominator. */
    for (idx_t i = 0; i < r; ++i) {
        for (idx_t j = 0; j < r; ++j) {
            seq_t g = gcd(num[i], den[j]);
            if (g != (seq_t)1) {
                num[i] /= g;
                den[j] /= g;
                if (den[j] == (seq_t)1)
                    break;
            }
        }
    }

    /* Multiply all remaining numerators. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < r; ++i)
        result = result * num[i];

    /* Divide by the remaining denominators.  After cancellation all
       denominators should divide the product exactly. */
    for (idx_t i = 0; i < r; ++i)
        if (den[i] != (seq_t)1)
            result = result / den[i];

    free(num);
    free(den);
    return result;
}

seq_t a(idx_t n)
{
    /* Map the flat index n to the corresponding row k and column r
       in the Gaussian q‑binomial triangle. */
    idx_t m = ( __int128)8 * n + 1;          /* 8*n + 1  (fits in 64 bit for the n values tested) */
    idx_t L = _floor_isqrt_u64(m);           /* floor(sqrt(8*n+1)) */
    idx_t k = (L - 1) / 2;                   /* row number */
    idx_t t_k = k * (k + 1) / 2;             /* number of elements before row k */
    idx_t r = n - t_k;                       /* column number in the row */
    return gaussian_q_binomial(k, r);
}
