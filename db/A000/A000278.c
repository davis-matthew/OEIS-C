/*
 * A000278  a(n) = a(n-1) + a(n-2)^2 for n >= 2 with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A000278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2)^2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 0; /* a(i-1) */
    seq_t cur = 1;  /* a(i) */
    idx_t i;

    if (n == 0)
        return (seq_t)0;
    for (i = 1; i < n; i++) {
        seq_t next = cur + prev * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
