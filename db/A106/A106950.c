/*
 * A106950  Primes of the form x^2 + 18y^2.
 * https://oeis.org/A106950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1606, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106950: primes of the form x^2 + 18 y^2.
   Primes are exactly the primes ≡ 1 or 19 (mod 24). This code
   enumerates primes in increasing order and returns the n‑th one
   (offset 1). The implementation uses a deterministic Miller–Rabin
   test for 64‑bit values. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 128‑bit multiplication modulo mod (safe for 64‑bit operands) */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

/* Modular exponentiation: a^e mod mod */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t result = 1;
    a %= mod;
    while (e)
    {
        if (e & 1U)
            result = modmul64(result, a, mod);
        a = modmul64(a, a, mod);
        e >>= 1U;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;

    /* Small prime division */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (unsigned int i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1U;
    unsigned int s = 0U;
    while ((d & 1U) == 0U)
    {
        d >>= 1U;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (unsigned int i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1U || x == n - 1U) continue;
        bool composite = true;
        for (unsigned int r = 1U; r < s; ++r)
        {
            x = modmul64(x, x, n);
            if (x == n - 1U)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Return the n‑th prime of the form x² + 18 y² (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* dummy for offset 1 */
    uint64_t count = 0U;
    uint64_t cand   = 5ULL;                 /* first odd > 3 */

    while (true)
    {
        if (is_prime_uint64(cand) &&
            ((cand % 24U == 1U) || (cand % 24U == 19U)))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 2U;                         /* skip even numbers */
    }
}
