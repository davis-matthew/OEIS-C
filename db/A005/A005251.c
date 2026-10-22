/*
 * A005251  a(0) = 0, a(1) = a(2) = a(3) = 1; thereafter, a(n) = a(n-1) +
 *          a(n-2) + a(n-4).
 * https://oeis.org/A005251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4), a(0) = 0, a(1) = a(2) = a(3) =
 *            1
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 81, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t w, x, y, z, t;
    idx_t i;

    if (n == 0) return (seq_t)0;
    if (n < 4) return (seq_t)1;

    w = (seq_t)0; /* a(i-4) */
    x = (seq_t)1; /* a(i-3) */
    y = (seq_t)1; /* a(i-2) */
    z = (seq_t)1; /* a(i-1) */
    for (i = 4; i <= n; i++) {
        t = z + y + w;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
