/*
 * A057454  n*M31 + 1 is prime, where M31 = 2^31 - 1.
 * https://oeis.org/A057454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that (k·M31 + 1) is
 *            prime, where M31 = 2^31 – 1 = 2147483647
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static const uint64_t M31 = 2147483647ULL;

/* 128‑bit multiplication modulo a 64‑bit modulus to avoid overflow. */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* Fast modular exponentiation (a^e mod mod). */
static uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (e)
    {
        if (e & 1ULL)
            res = mul_mod64(res, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1ULL;
    }
    return res;
}

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers.
   The base set {2, 325, 9375, 28178, 450775, 9780504, 1795265022} is
   correct for all 64‑bit values. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Small prime trial division – speeds up very small numbers. */
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0ULL) return false;
    }

    /* Write n–1 as d·2^s with d odd. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Return the n‑th term of sequence A057454. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (true)
    {
        /* Compute k·M31 + 1 safely in 128‑bit arithmetic. */
        unsigned __int128 prod = (unsigned __int128)k * M31 + 1;
        /* The sequence is designed to stay well within 64 bits
           for the range of n tested. */
        uint64_t candidate = (uint64_t)prod;

        if (is_prime_uint64(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
