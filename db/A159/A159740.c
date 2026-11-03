/*
 * A159740  Number of permutations of 9 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159740
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*n*10^(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t ten_pow = ipow_idx((seq_t)10, n - 2);
    return (seq_t)9 * ((seq_t)n) * ten_pow;
}
