/*
 * A102572  a(n) = floor(log_4(n)).
 * https://oeis.org/A102572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_4(n))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_log2_idx_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_idx_helper.h"
/* Implements the sequence using only integer arithmetic. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, return 0 for safety */
    idx_t l2 = floor_log2_idx_helper(n);
    return (seq_t)(l2 >> 1);     /* divide by 2: floor(log2(n)/2) = floor(log4(n)) */
}
