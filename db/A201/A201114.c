/*
 * A201114  Primes of the form 3n^3+4.
 * https://oeis.org/A201114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form 3*n^3 + 4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 81, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* 64‑bit Miller–Rabin primality test (deterministic for n < 2^64)    */
/* ------------------------------------------------------------------ */

static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t result = 1;
    unsigned __int128 base = a % mod;
    while (e)
    {
        if (e & 1ULL)
            result = (uint64_t)((unsigned __int128)result * base % mod);
        base = (base * base) % mod;
        e >>= 1ULL;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Small primes – rule out trivial composites. */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    /* Miller–Rabin with the 7 bases that are deterministic for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r)
        {
            x = (uint64_t)((unsigned __int128)x * x % n);
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

/* ------------------------------------------------------------------ */
/* Cached generation of the sequence.                                 */
/* ------------------------------------------------------------------ */

static seq_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cap  = 0;        /* allocated capacity   */
static size_t prime_len  = 0;        /* number of stored primes */
static idx_t  candidate_n = 1;       /* next integer n to try */

/* Expand the cache until it contains at least 'need' primes. */
static void cache_ensure(idx_t need)
{
    while (prime_len < (size_t)need)
    {
        /* Compute 3*n^3 + 4 in 128‑bit to avoid overflow for small n. */
        unsigned __int128 t = candidate_n;
        t = t * candidate_n * candidate_n;  /* t = n^3            */
        t = t * 3 + 4;                       /* t = 3*n^3 + 4     */

        uint64_t val64 = (uint64_t)t;        /* fits into 64 bits
                                              for all candidates used */
        seq_t val = (seq_t)val64;

        if (is_prime_uint64(val64))
        {
            /* Grow the array if necessary. */
            if (prime_len == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                              new_cap * sizeof(seq_t));
                /* realloc may return NULL only if new_cap==0, which
                   never happens here. */
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_len++] = val;
        }
        ++candidate_n;
    }
}

/* ------------------------------------------------------------------ */
/* Public API required by the OEIS test harness.                      */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined offset, but return 0 to be safe */
        return 0;
    cache_ensure(n);
    return prime_cache[n - 1];
}
