/*
 * A084610  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1+x-x^2)^n.
 * https://oeis.org/A084610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 66, i16 n <= 263, i32 n <= 923, i64 n <= 1224,
 *            i128 n <= 1224, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the coefficient of x^k in (1 + x - x^2)^r,
 * where r = floor(sqrt(n)) and k = n - r^2.  The coefficient
 * T(r, k) is computed by
 *     T(r, k) = Σ_j=0^k C(r, k-j)·C(k-j, j)·(-1)^j
 * where the sum is effectively over j from max(0, k-r) to k/2.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find r = floor(sqrt(n)) and k = n - r^2 */
    idx_t r = (idx_t)sqrt((long double)n);
    /* correct possible rounding errors */
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    idx_t k = n - r * r;

    /* compute the trinomial coefficient by the given sum */
    idx_t j_start = (k > r) ? (k - r) : 0;
    idx_t j_end   = k >> 1;             /* floor(k/2) */
    seq_t sum = 0;

    for (idx_t j = j_start; j <= j_end; ++j) {
        seq_t term = binom(r, k - j);          /* C(r, k-j) */
        term *= binom(k - j, j);               /* * C(k-j, j) */
        if (j & 1)
            sum -= term;                       /* (-1)^j */
        else
            sum += term;
    }
    return sum;
}
