/*
 * A065109  Triangle T(n,k) of coefficients relating to Bezier curve
 *          continuity.
 * https://oeis.org/A065109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice, easy
 * Formula:   a(n) = (-1)^k * 2^(r-k) * binomial(r,k) where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2
 * Range:     i8 n <= 21, i16 n <= 68, i32 n <= 257, i64 n <= 914,
 *            i128 n <= 3508, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column k by counting rows */
    idx_t r = 0;
    idx_t tri = 0;                     /* total elements before row r */
    while (n >= tri + r + 1) {
        r++;
        tri += r;                      /* tri now equals r*(r+1)/2 */
    }
    idx_t k = n - tri;

    /* Compute binomial coefficient */
    seq_t res = binomial(r, k);

    /* Multiply by 2^(r-k) */
    for (idx_t i = 0; i < r - k; ++i)
        res *= (seq_t)2;

    /* Apply (-1)^k */
    if (k & 1)
        res = -res;

    return res;
}
