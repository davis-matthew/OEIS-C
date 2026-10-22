/*
 * A000261  a(n) = n*a(n-1) + (n-3)*a(n-2), with a(1) = 0, a(2) = 1.
 * https://oeis.org/A000261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-3)*a(n-2), a(1) = 0, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 0; /* a(1) */
    seq_t cur = 1;  /* a(2) */
    idx_t i;

    if (n <= 1)
        return prev;
    if (n == 2)
        return cur;

    for (i = 3; i <= n; i++) {
        seq_t next = (seq_t)i * cur + (seq_t)(i - 3) * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
