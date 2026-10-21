/*
 * A050449  a(n) = Sum_{d|n, d == 1 (mod 4)} d.
 * https://oeis.org/A050449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d ≡ 1 (mod 4)} d
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 % 4 == 1)
                sum += (seq_t)d1;
            if (d2 != d1 && d2 % 4 == 1)
                sum += (seq_t)d2;
        }
    }
    return sum;
}
