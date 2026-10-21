/*
 * A213980  a(n) is the minimum number of iterations of f(n) = n + Omega(n) -
 *          1 that results in a prime number, where Omega(n) is the number of
 *          prime factors of n, counted with multiplicity, or A001222(n).
 * https://oeis.org/A213980
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of iterations of f(n) = n + Omega(n) – 1
 *            until the value is prime
 * Range:     u8 n <= 462, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Omega counts prime factors with multiplicity) */
#include <stdint.h>
#include <stdbool.h>

/* trial‑division primality test for 64‑bit numbers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* count prime factors with multiplicity (Omega) for 64‑bit numbers */
static uint64_t omega_uint64(uint64_t x)
{
    uint64_t cnt = 0;
    while ((x & 1ULL) == 0) {  /* divide out factors of 2 */
        ++cnt;
        x >>= 1;
    }
    for (uint64_t d = 3; d * d <= x; d += 2) {
        while (x % d == 0) {
            ++cnt;
            x /= d;
        }
    }
    if (x > 1) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 2 by the specification */
    seq_t cur = (seq_t)n;
    seq_t steps = 0;
    while (!is_prime_uint64((uint64_t)cur)) {
        uint64_t om = omega_uint64((uint64_t)cur);
        cur = cur + (seq_t)om - 1;
        ++steps;
    }
    return steps;
}
