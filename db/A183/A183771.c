/*
 * A183771  1/32 the number of (n+1) X 9 binary arrays with equal numbers of 2
 *          X 2 subblocks with sum mod two being 0 and 1.
 * https://oeis.org/A183771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+4) * C(8n, 4n)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow2(n + 4);
    seq_t bin_val  = binom(8 * n, 4 * n);
    return pow_val * bin_val;
}
