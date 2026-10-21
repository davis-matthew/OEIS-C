/*
 * A094347  a(n) = 14*a(n-1) - a(n-2); a(0) = a(1) = 2.
 * https://oeis.org/A094347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2); a(0) = a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)2;

    seq_t prev_prev = (seq_t)2; /* a(n-2) */
    seq_t prev      = (seq_t)2; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)14 * prev - prev_prev;
        prev_prev = prev;
        prev = cur;
    }
    return prev;
}
