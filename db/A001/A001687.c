/*
 * A001687  a(n) = a(n-2) + a(n-5).
 * https://oeis.org/A001687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-5), a(0) = 0, a(1) = 1, a(2) = 0, a(3) = 1,
 *            a(4) = 0
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 110, u64 n <= 215,
 *            u128 n <= 424, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t buf[5];
    buf[0] = 0;
    buf[1] = 1;
    buf[2] = 0;
    buf[3] = 1;
    buf[4] = 0;
    if (n < 5) return buf[n];
    for (idx_t i = 5; i <= n; i++) {
        /* buf[i%5] holds a(i-5), buf[(i+3)%5] holds a(i-2) */
        buf[i % 5] += buf[(i + 3) % 5];
    }
    return buf[n % 5];
}
