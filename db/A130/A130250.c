/*
 * A130250  Minimal index k of a Jacobsthal number such that A001045(k) >= n
 *          (the 'upper' Jacobsthal inverse).
 * https://oeis.org/A130250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌈log₂(3n − 1)⌉ for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 85, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   ceil_log2_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_log2_seq_t.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0u;

    /* val = 3*n - 1, computed in seq_t to avoid overflow on large idx_t */
    seq_t val = (seq_t)n * (seq_t)3u - (seq_t)1u;
    return ceil_log2_seq_t(val);
}
