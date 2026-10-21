/*
 * A201782  Primes of the form 3n^2 - 10.
 * https://oeis.org/A201782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 3k^2 – 10 (k counted from 1)
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 2466, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

/* static cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;                         /* k to test next */

/* ensure cache capacity */
static void grow_cache(size_t new_len)
{
    if (cache_cap < new_len) {
        size_t new_cap = cache_cap ? cache_cap : 256;
        while (new_cap < new_len)
            new_cap *= 2;
        /* seq_realloc moves contents and frees old block */
        cache = seq_realloc(cache, cache_cap, new_cap);
        if (!cache)
            abort();
        cache_cap = new_cap;
    }
}

/* sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, return 0 */

    /* generate primes until we have at least n elements */
    while (cache_len < (size_t)n) {
        seq_t nk = (seq_t)next_k;
        seq_t val = (seq_t)3 * nk * nk - (seq_t)10;
        if (isPrimeGeneric(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_k;
    }
    return cache[n - 1];
}
