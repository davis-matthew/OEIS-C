/*
 * A073933  Number of terms in n-th row of triangle in A073932.
 * https://oeis.org/A073933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 1 + a(n - n/spf(n)) for n > 1, where spf is the
 *            smallest prime factor (n - n/spf(n) = A060681(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9107 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A073933_step(idx_t m)
{
    /* m > 1: m - largest proper divisor (m-1 if m is prime) */
    if ((m & 1) == 0)
        return m - m / 2;
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0)
            return m - m / i;
    }
    return m - 1;
}

seq_t a(idx_t n)
{
    seq_t count = 1;
    idx_t m = n;
    while (m > 1) {
        m = A073933_step(m);
        ++count;
    }
    return count;
}
