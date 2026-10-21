/*
 * A050455  a(n) = Sum_{d|n, d=3 mod 4} d^4.
 * https://oeis.org/A050455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | n, d ≡ 3 (mod 4)} d^4
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 258, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (d % 4 == 3) {
                seq_t dd = (seq_t)d;
                seq_t d2 = dd * dd;
                seq_t d4 = d2 * d2;
                sum += d4;
            }
            idx_t e = n / d;
            if (e != d && e % 4 == 3) {
                seq_t ee = (seq_t)e;
                seq_t e2 = ee * ee;
                seq_t e4 = e2 * e2;
                sum += e4;
            }
        }
    }
    return sum;
}
