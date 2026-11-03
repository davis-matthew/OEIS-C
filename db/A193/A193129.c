/*
 * A193129  Number of spanning trees of the n-barbell graph.
 * https://oeis.org/A193129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 1 for n=0,1,2; a(n) = n^(2n-4) for n>=3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;
    idx_t exp = 2 * n - 4;
    return ipow_u64_idx((uint64_t)n, exp);
}
