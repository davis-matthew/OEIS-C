/*
 * A104507  Row sums of triangle A104505, which is equal to the right-hand
 *          side of the triangle A084610 of coefficients in (1+x-x^2)^n.
 * https://oeis.org/A104507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{j=0..⌊n/2⌋} (−1)^j · binomial(n,j) · binomial(n−j−1,
 *            n−2j)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 29, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return 1;

    seq_t sum = 0;
    /* j runs from 0 to floor(n/2) */
    for (idx_t j = 0; j <= n / 2; ++j) {
        seq_t term = binomial(n, j) * binomial(n - j - 1, n - 2 * j);
        if (j & 1)        /* odd j → subtract */
            sum -= term;
        else              /* even j → add   */
            sum += term;
    }
    return sum;
}
