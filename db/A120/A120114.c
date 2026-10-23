/*
 * A120114  a(n) = lcm(1, ..., 2n+4)/lcm(1, ..., 2n+2).
 * https://oeis.org/A120114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(1,…,2n+4)/lcm(1,…,2n+2) = product of primes p such
 *            that 2n+3 or 2n+4 is a power of p
 * Range:     u8 n <= 126, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the prime p if m is a power of p (including the case m is a prime);
   otherwise return 0. */
static uint64_t prime_power_factor(uint64_t m)
{
    if (m <= 1) return 0;
    uint64_t p = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) { p = d; break; }
    }
    if (p == 0) return m;          /* m is prime */
    while (m % p == 0) m /= p;
    return (m == 1) ? p : 0;       /* power of p? */
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    uint64_t m1 = 2 * (uint64_t)n + 3;
    uint64_t m2 = 2 * (uint64_t)n + 4;
    uint64_t p1 = prime_power_factor(m1);
    uint64_t p2 = prime_power_factor(m2);
    if (p1) res *= (seq_t)p1;
    if (p2) res *= (seq_t)p2;
    return res;
}
