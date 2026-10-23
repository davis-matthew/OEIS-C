/*
 * A181478  a(n) has generating function 1/((1-x)^k*(1-x^2)^(k*(k-1)/2)) for
 *          k=6.
 * https://oeis.org/A181478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0..⌊n/2⌋} C(n-2j+5,5) * C(j+14,14) (k = 6)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 25, u64 n <= 110, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t jmax = n >> 1;           /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t j = 0; j <= jmax; ++j) {
        idx_t p   = n - (j << 1);   /* n - 2j */
        seq_t term = binom(p + 5, 5) * binom(j + 14, 14);
        sum += term;
    }
    return sum;
}
