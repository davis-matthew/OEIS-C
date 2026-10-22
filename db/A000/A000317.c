/*
 * A000317  a(n+1) = a(n)^2 - a(n)*a(n-1) + a(n-1)^2.
 * https://oeis.org/A000317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 - a(n)*a(n-1) + a(n-1)^2, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 1;   /* a(1) */
    seq_t cur = 2;    /* a(2) */
    idx_t i;

    if (n <= 1)
        return prev;
    for (i = 2; i < n; i++) {
        /* a(n) >= a(n-1), so a*(a-b) never goes negative */
        seq_t next = cur * (cur - prev) + prev * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
