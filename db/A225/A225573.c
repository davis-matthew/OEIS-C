/*
 * A225573  Number of trees over all forests of labeled rooted trees in which
 *          some (possibly all or none) of the trees have been specially
 *          designated.
 * https://oeis.org/A225573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n*(n+2)^(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)12;

    /* compute (n+2)^(n-2) using pow_uint, then multiply */
    seq_t val = ipow_u64_idx((uint64_t)(n + 2), n - 2);
    val = val * (seq_t)n * (seq_t)6;
    return val;
}
