/*
 * A074595  Number of prime factors of numbers that are not prime powers (with
 *          multiplicity).
 * https://oeis.org/A074595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A024619(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9032 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Number of prime factors with multiplicity of the nth non‑prime‑power) */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff x is a prime power (including primes). */
static bool is_prime_power(uint64_t x)
{
    uint64_t p = 0;
    for (uint64_t i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            p = i;
            break;
        }
    }
    if (p == 0)          /* x is prime */
        return true;
    while (x % p == 0)
        x /= p;
    return x == 1;       /* remaining factor is 1 → pure power */
}

/* Count prime factors of n with multiplicity (Ω(n)). */
static int count_prime_factors(uint64_t n)
{
    int cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    if (n > 1) /* final prime factor */
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    idx_t target = n;
    idx_t count  = 0;
    uint64_t m   = 2;

    while (1) {
        if (!is_prime_power(m)) {
            ++count;
            if (count == target)
                break;
        }
        ++m;
    }

    return (seq_t)count_prime_factors(m);
}
