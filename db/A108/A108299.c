/*
 * A108299  Triangle read by rows, 0 <= k <= n: T(n,k) =
 *          binomial(n-[(k+1)/2],[k/2])*(-1)^[(k+1)/2].
 * https://oeis.org/A108299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = binomial(r - floor((k+1)/2), floor(k/2)) *
 *            (-1)^floor((k+1)/2)
 * Range:     i8 n <= 110, i16 n <= 362, i32 n <= 1250, i64 n <= 4705,
 *            i128 n <= 9126, big any n
 * Helpers:   binom, binom_safe, integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is read by rows; a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), … */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/integer_sqrt.h"
#include "../utils/binom_safe.h"

seq_t a(idx_t n)
{
    /* locate row r and column k */
    idx_t r = (integer_sqrt(8 * n + 1) - 1) / 2;
    while (r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    idx_t k = n - r * (r + 1) / 2;

    /* halves for the formula */
    idx_t half1 = (k + 1) / 2;   /* floor((k+1)/2) */
    idx_t half2 = k / 2;         /* floor(k/2) */

    /* binomial part */
    idx_t baseN = r - half1;
    idx_t baseK = half2;
    seq_t val = binom_safe(baseN, baseK);

    /* apply sign */
    return (half1 & 1) ? -val : val;
}
