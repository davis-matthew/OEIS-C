/*
 * A105348  An indicator sequence for the Jacobsthal numbers.
 * https://oeis.org/A105348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(log_2(3*n+1)) - floor(log_2(3*n-2)), a(0) = 1
 * Range:     u8 n <= 84, u16 n <= 8865, u32 n <= 8865, u64 n <= 8865,
 *            u128 n <= 8865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute floor(log2(x)) for positive x. */
static seq_t floor_log2_seq(seq_t x)
{
    idx_t shift = 0;
    while ((x >> 1) != 0) {
        x >>= 1;
        ++shift;
    }
    return (seq_t)shift;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t m3 = (seq_t)n * 3;
    seq_t x1 = m3 + 1;   /* 3*n + 1 */
    seq_t x2 = m3 - 2;   /* 3*n - 2 (n >= 1, so positive) */
    seq_t l1 = floor_log2_seq(x1);
    seq_t l2 = floor_log2_seq(x2);
    return l1 - l2;
}
