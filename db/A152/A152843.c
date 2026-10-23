/*
 * A152843  Numbers n such that both 2n+3 and 4n+7 are prime.
 * https://oeis.org/A152843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer m such that 2m+3 and 4m+7 are prime
   The function builds the sequence by incremental search and caches
   previously found terms.  It works for any seq_t / idx_t types. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Miller–Rabin for 64‑bit numbers ----------
   Uses the known deterministic bases for the 64‑bit range. */
static uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mulmod_u64(result, base, mod);
        base = mulmod_u64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* check small primes first */
    static const uint64_t small_primes[] =
    {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(*small_primes); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }
    /* write n‑1 = d · 2ⁿ with d odd */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }
    /* deterministic bases */
    static const uint64_t bases[] =
    {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(*bases); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r)
        {
            x = mulmod_u64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Cache of computed terms (stored as idx_t to avoid C++ allocation issues). */
    static idx_t *cache = NULL;   /* array of terms  */
    static size_t cache_size = 0; /* number of stored terms  */
    static size_t cache_cap   = 0; /* allocated capacity       */
    static idx_t  next_test = 0;   /* next integer to examine */

    /* Grow cache until it contains at least n terms. */
    while (cache_size < n)
    {
        /* Test current next_test value. */
        uint64_t val1 = 2ULL * (uint64_t)next_test + 3ULL;
        uint64_t val2 = 4ULL * (uint64_t)next_test + 7ULL;
        if (is_prime_uint64(val1) && is_prime_uint64(val2))
        {
            /* Ensure room for a new entry. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                idx_t *tmp = (idx_t *)malloc(new_cap * sizeof(idx_t));
                if (!tmp)
                    abort(); /* allocation failure */
                if (cache)
                {
                    memcpy(tmp, cache, cache_size * sizeof(idx_t));
                    free(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_test;
        }
        ++next_test;
    }

    return (seq_t)cache[n - 1U];
}
