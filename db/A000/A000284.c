/*
 * A000284  a(n) = a(n-1)^3 + a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A000284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^3 + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 0;   /* a(i)   */
    seq_t cur = 1;    /* a(i+1) */
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t next = cur * cur * cur + prev;
        prev = cur;
        cur = next;
    }
    return prev;
}
