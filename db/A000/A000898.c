/*
 * A000898  a(n) = 2*(a(n-1) + (n-1)*a(n-2)) for n >= 2 with a(0) = 1.
 * https://oeis.org/A000898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*(a(n-1) + (n-1)*a(n-2)), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev = 1;   /* a(0) */
    seq_t cur = 2;    /* a(1) */
    idx_t i;
    if (n == 0) return prev;
    for (i = 2; i <= n; i++) {
        seq_t next = 2 * (cur + (seq_t)(i - 1) * prev);
        prev = cur;
        cur = next;
    }
    return cur;
}
