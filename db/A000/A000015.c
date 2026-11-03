/*
 * A000015  Smallest prime power >= n.
 * https://oeis.org/A000015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search upward from n for the first m that is 1 or a prime power
 *            (p^k, k >= 1)
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9587 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_power_ull(unsigned long long m)
{
    unsigned long long p;
    if (m <= 1) return m == 1;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            return m == 1;
        }
    }
    return true; /* m is prime */
}

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n;
    if (m < 1) m = 1;
    while (!is_prime_power_ull(m)) m++;
    return (seq_t)m;
}
