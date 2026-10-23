/*
 * A201300  Primes of the form 9n^3+5.
 * https://oeis.org/A201300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 9*n^3+5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* Modulo multiplication using 128‑bit intermediate to avoid overflow. */

/* Modulo exponentiation. */
static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1U) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1U;
    }
    return r;
}

/* Deterministic Miller‑Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime division test. */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (unsigned i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0ULL) return n == p;
    }

    /* Write n-1 as d * 2^s with d odd. */
    uint64_t d = n - 1U;
    unsigned int s = 0U;
    while ((d & 1U) == 0U)
    {
        d >>= 1U;
        ++s;
    }

    /* Deterministic bases for 64‑bit. */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (unsigned i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0ULL) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1U) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1U) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Return the n‑th prime of the form 9*k^3+5.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    idx_t count = 0U;
    uint64_t k = 0ULL;

    while (true)
    {
        /* Only even k yield odd candidates. */
        if ((k & 1ULL) == 0ULL)
        {
            unsigned __int128 cube = (unsigned __int128)k * k * k;
            unsigned __int128 val128 = 9U * cube + 5U;
            uint64_t cand = (uint64_t)val128;       /* fits in 64 bits */
            if (is_prime_uint64(cand))
            {
                ++count;
                if (count == n) return (seq_t)cand;
            }
        }
        ++k;
    }
}
