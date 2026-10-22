/*
 * A001910  a(n) = n*a(n-1) + (n-5)*a(n-2).
 * https://oeis.org/A001910
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-5)*a(n-2), a(3) = 0, a(4) = 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev = 0;   /* a(3) */
    seq_t cur = 1;    /* a(4) */
    idx_t i;
    if (n <= 3) return prev;
    if (n == 4) return cur;
    for (i = 5; i <= n; i++) {
        seq_t next = (seq_t)i * cur + (seq_t)(i - 5) * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
