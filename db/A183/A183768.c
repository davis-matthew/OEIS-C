/*
 * A183768  1/32 the number of (2n+1) X 6 binary arrays with equal numbers of
 *          2 X 2 subblocks with sum mod two being 0 and 1.
 * https://oeis.org/A183768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n+1) * binom(10n, 5n)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute 2^(2n+1) */
    idx_t exp   = 2 * n + 1;
    seq_t pow2  = 1;
    for (idx_t i = 0; i < exp; ++i) {
        pow2 *= 2;
    }

    /* compute binomial coefficient binom(10n, 5n) */
    seq_t bin = binom(10 * n, 5 * n);

    /* return the product */
    return pow2 * bin;
}
