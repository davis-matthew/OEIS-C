/*
 * A005831  a(n+1) = a(n) * (a(n-1) + 1).
 * https://oeis.org/A005831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) * (a(n-2) + 1), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev = 0;  /* a(0) */
    seq_t cur = 1;   /* a(1) */
    idx_t i;

    if (n == 0) {
        return prev;
    }
    for (i = 2; i <= n; i++) {
        seq_t next = cur * (prev + 1);
        prev = cur;
        cur = next;
    }
    return cur;
}
