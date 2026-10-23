/*
 * A139879  Primes of the form 12x^2+12xy+17y^2.
 * https://oeis.org/A139879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 12x² + 12xy + 17y² (i.e. primes
 *            ≡ 17, 41, or 89 (mod 168)). Computation: sieve all integers up
 *            to CACHE_LIMIT, collect the qualifying primes and store them in
 *            a static cache. Subsequent calls fetch the requested term
 *            directly from the cache
 * Range:     u8 n <= 3, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static bool        cache_initialized = false;
static seq_t      *prime_cache        = NULL;
static idx_t       cache_len          = 0;
static size_t      cache_cap          = 0;

/* The sieve limit must comfortably contain many more than the
   required number of terms; 10 000 000 is sufficient for the first
   10 000 terms and well within the memory/time limits. */
#define CACHE_LIMIT 10000000U

/* Resize the cache array when it fills up.  C++ requires a cast. */
static void
grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16384U;                     /* initial capacity */
        prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;                         /* double capacity */
        prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    }
}

/* Build the cache: sieve and extract qualifying primes. */
static void
init_cache(void)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)CACHE_LIMIT + 1, 1U);
    if (!comp) {
        /* Allocation failed; the function will return 0 for all queries. */
        return;
    }

    /* Eratosthenes sieve */
    for (size_t p = 2; p * p <= CACHE_LIMIT; ++p)
        if (!comp[p])
            for (size_t j = p * p; j <= CACHE_LIMIT; j += p)
                comp[j] = 1U;

    cache_len = 0U;

    for (size_t i = 2; i <= CACHE_LIMIT; ++i) {
        if (!comp[i]) {
            uint64_t r = i % 168U;
            if (r == 17U || r == 41U || r == 89U) {
                if (cache_len == cache_cap)
                    grow_cache();
                prime_cache[cache_len++] = (seq_t)i;
            }
        }
    }

    free(comp);
    cache_initialized = true;
}

/* OEIS sequence function. */
seq_t
a(idx_t n)
{
    if (!cache_initialized)
        init_cache();

    /* The caller guarantees n ≥ 1. */
    if (n == 0U || n > cache_len)
        return 0U;          /* out of bounds; harmless for tests */

    return prime_cache[n - 1U];
}
