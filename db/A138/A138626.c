/*
 * A138626  Nonnegative integers n such that 17*n-5 is prime.
 * https://oeis.org/A138626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 315, u32 n <= 355, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th nonnegative integer k such that 17*k‑5 is prime (OEIS A138626) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* static cache of discovered terms */
static seq_t *cache = NULL;
static size_t cache_size   = 0;   /* number of terms stored */
static size_t cache_cap    = 0;   /* allocated capacity */
static idx_t  test_candidate = 2; /* next candidate value to test */

/* ensure the cache has room for at least 'need' elements */
static void
ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    /* generate terms until we have at least 'n' of them */
    while (cache_size < (size_t)n) {
        /* compute 17 * test_candidate – 5 in a type wide enough to avoid
           overflow for the expected ranges */
        unsigned __int128 val128 = (unsigned __int128)17 * test_candidate;
        val128 -= 5;
        seq_t val = (seq_t)val128;  /* truncation is safe for the expected range */

        if (is_prime_big(val)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = test_candidate;
        }
        ++test_candidate;
    }

    return cache[(size_t)n - 1];
}
