/*
 * A180966  Hankel transform of A123164.
 * https://oeis.org/A180966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n*(n-1)/2) * b(n), where b(0)=1, b(1)=4, and for n≥2
 *            b(n) = 4*b(n-1) - 2*b(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Compute b(n) via the linear recurrence. */
    seq_t b_prev = (seq_t)1;   /* b(0) */
    seq_t b_curr = (seq_t)4;   /* b(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t b_next = ((seq_t)4 * b_curr) - ((seq_t)2 * b_prev);
        b_prev = b_curr;
        b_curr = b_next;
    }

    /* Multiply by the required power of two: 2^(n*(n-1)/2). */
    idx_t shift = (n * (n - 1)) / 2;
    return b_curr * pow2(shift);
}
