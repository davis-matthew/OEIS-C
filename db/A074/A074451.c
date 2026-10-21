/*
 * A074451  Non-cubefree noncubes.
 * https://oeis.org/A074451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer that is divisible by a prime
 *            cube but is not a perfect cube
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   icbrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/icbrt_uint64.h"

/* Pre‑computed small primes used to test for prime cubes. */
static int primes[50];
static size_t prime_cnt = 0;

/* Initialize the primes array once. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    bool sieve[201] = { false };
    for (int i = 2; i <= 200; ++i) sieve[i] = true;
    for (int i = 2; i * i <= 200; ++i)
        if (sieve[i]) for (int j = i * i; j <= 200; j += i) sieve[j] = false;
    for (int i = 2; i <= 200; ++i) if (sieve[i]) primes[prime_cnt++] = i;
    done = true;
}

/* Cached sequence values. */
static seq_t *cache     = NULL;  /* pointer to array of seq_t */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* current allocation capacity */
static uint64_t last_checked = 0;/* last integer that was examined */

/* Ensure that cache holds at least n_needed terms. */
static void ensure_cache(size_t n_needed)
{
    init_primes();
    while (cache_len < n_needed)
    {
        ++last_checked;
        uint64_t cand = last_checked;

        /* Skip perfect cubes. */
        uint64_t k = icbrt_uint64(cand);
        if ( (__int128)k * k * k == cand ) continue;

        /* Test whether cand has a prime factor with exponent ≥ 3. */
        bool has_cube_factor = false;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            int p = primes[i];
            uint64_t p3 = (uint64_t)p * p * p;
            if (p3 > cand) break;          /* p^3 cannot divide cand */
            if (cand % p3 == 0) { has_cube_factor = true; break; }
        }
        if (!has_cube_factor) continue;

        /* Store the candidate in the cache. */
        if (cache_len == cache_cap)
        {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache,
                cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1: undefined for 0, guard against misuse */
    ensure_cache((size_t)n);
    return cache[n - 1];
}
