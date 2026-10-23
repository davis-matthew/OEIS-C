/*
 * A153338  Number of zig-zag paths from top to bottom of a 2n-1 by 2n-1
 *          square whose color is not that of the top right corner.
 * https://oeis.org/A153338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*2^(2*n-2) - (2*n-1)*binomial(2*n-2, n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A153338. */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* out of range, return 0 */
    /* Compute the two terms of the formula */
    seq_t term1 = (seq_t)n * pow2(2 * n - 2);
    seq_t bin = binom(2 * n - 2, n - 1);
    seq_t term2 = (seq_t)(2 * n - 1) * bin;
    return term1 - term2;
}
