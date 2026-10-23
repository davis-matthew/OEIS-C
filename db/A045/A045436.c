/*
 * A045436  Primes congruent to {1, 3} mod 7.
 * https://oeis.org/A045436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: primes congruent to 1 or 3 mod 7
   a(n) is the n‑th prime p with p mod 7 == 1 or 3.
   Offset: 1
*/

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ----------------------------------------------------------------------------- */
/*  Cached list of primes ≡1 or 3 (mod 7)                                      */
/* ----------------------------------------------------------------------------- */

static seq_t *prime_cache = NULL;   /* array of cached primes            */
static size_t  cache_len   = 0;      /* number of primes stored           */
static size_t  cache_cap   = 0;      /* allocated capacity                */
static idx_t   cached_limit = 0;     /* sieve limit used for current cache*/

/* ----------------------------------------------------------------------------- */
/*  Estimate an upper bound for the n‑th prime ≡1 or 3 (mod 7)                 */
/* ----------------------------------------------------------------------------- */

static idx_t estimate_limit(idx_t n)
{
    if (n == 0) return 7;               /* trivial safe limit            */
    /* density ≈ 2/7; use the prime number theorem with a safety factor   */
    double nn = (double)n * 3.5;                /* n·7/2 */
    double ub = nn * (log(nn) + log(log(nn))); /* Rosser–Schoenfeld bound   */
    return (idx_t)(ub + 20.0);                /* additional safety margin   */
}

/* ----------------------------------------------------------------------------- */
/*  Rebuild the cache up to a given sieve limit                                 */
/* ----------------------------------------------------------------------------- */

static void rebuild_cache(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    unsigned char *sieve = (unsigned char *)malloc(sz);
    if (!sieve) return;                 /* out of memory: silently fail  */

    memset(sieve, 0, sz);
    sieve[0] = sieve[1] = 1;

    for (idx_t p = 2; (p * p) <= limit; ++p)
        if (!sieve[p])
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = 1;

    /* Reset cache and grow if necessary                                 */
    cache_len = 0;
    if (cache_cap == 0)
    {
        cache_cap = 64; /***** default capacity *****/
        prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
        if (!prime_cache) { free(sieve); return; }
    }

    for (idx_t v = 2; v <= limit; ++v)
    {
        if (!sieve[v] && ((v % 7) == 1 || (v % 7) == 3))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(sieve); return; }
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)v;
        }
    }

    cached_limit = limit;
    free(sieve);
}

/* ----------------------------------------------------------------------------- */
/*  Ensure that the cache contains at least n primes                            */
/* ----------------------------------------------------------------------------- */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;            /* already enough                           */
    idx_t needed_limit = estimate_limit(n);
    if (needed_limit <= cached_limit) return; /* shouldn't happen but safety check      */
    rebuild_cache(needed_limit);
}

/* ----------------------------------------------------------------------------- */
/*  Public sequence function                                                   */
/* ----------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 0                  */
    ensure_cache(n);
    return prime_cache[(size_t)n - 1];
}
