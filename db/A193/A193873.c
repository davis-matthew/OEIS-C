/*
 * A193873  Smallest product of three distinct primes of the form n*k+1.
 * https://oeis.org/A193873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the first three distinct primes of the form
 *            n*k+1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64-bit modular multiplication using built‑in 128‑bit type */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* 64-bit modular exponentiation */
static uint64_t pow_mod(uint64_t a, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    a %= mod;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2ULL) return false;
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0ULL) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) return true;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1ULL; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* OEIS A193873 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – undefined for n = 0 */

    seq_t product = 1;
    int found = 0;
    uint64_t k = 1;
    while (found < 3)
    {
        uint64_t m = (uint64_t)n * k + 1ULL;
        if (is_prime_u64(m))
        {
            product *= (seq_t)m;
            ++found;
        }
        ++k;
    }
    return product;
}
