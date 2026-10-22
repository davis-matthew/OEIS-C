/*
 * A002660  a(n) = Sum_{d|n, d <= 3} d^2 + 3*Sum_{d|n, d>3} d.
 * https://oeis.org/A002660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*sigma(n) - k, where k = 4 if n is even and k = 2 if n
 *            is odd
 * Range:     u8 n <= 35, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t d;
    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            s += (seq_t)d;
            if (e != d)
                s += (seq_t)e;
        }
    }
    return (seq_t)3 * s - ((n & 1) ? (seq_t)2 : (seq_t)4);
}
