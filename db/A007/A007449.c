/*
 * A007449  a(0) = 7, a(1) = 9; for n >= 0, a(2n+1) = a(2n-1)^2 - a(2n-2),
 *          a(2n+2) = a(2n)^2 - a(2n+1).
 * https://oeis.org/A007449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n+1) = a(2n-1)^2 - a(2n-2), a(2n+2) = a(2n)^2 - a(2n+1), a(0)
 *            = 7, a(1) = 9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p3 = 0;   /* a(i-3) */
    seq_t p2 = 7;   /* a(i-2) */
    seq_t p1 = 9;   /* a(i-1) */
    idx_t i;

    if (n == 0) return p2;
    if (n == 1) return p1;

    for (i = 2; i <= n; i++) {
        seq_t cur;
        if (i % 2 == 0) {
            cur = p2 * p2 - p1;
        } else {
            cur = p2 * p2 - p3;
        }
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
