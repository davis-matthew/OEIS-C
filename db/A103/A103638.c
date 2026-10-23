/*
 * A103638  a(n) = Sum_{d|n, d==2 mod 3} d^2.
 * https://oeis.org/A103638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d==2 mod 3} d^2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External types: idx_t is an unsigned integer type (uint64_t). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor i */
            if (i % 3 == 2) {
                seq_t di = (seq_t)i;
                sum += di * di;
            }
            /* paired divisor j = n / i */
            idx_t j = n / i;
            if (j != i && j % 3 == 2) {
                seq_t dj = (seq_t)j;
                sum += dj * dj;
            }
        }
    }
    return sum;
}
