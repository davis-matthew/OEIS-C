/*
 * A097053  First occurrence of n in A097051.
 * https://oeis.org/A097053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (largest power of 2 ≤ n) / 2, a(1) = 1
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   largest_pow2_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_pow2_le.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* compute highest power of 2 ≤ n */
    seq_t pow2 = (seq_t)largest_pow2_le(n);

    /* a(n) = n * pow2 / 2 */
    return ((seq_t)n * pow2) / 2;
}
