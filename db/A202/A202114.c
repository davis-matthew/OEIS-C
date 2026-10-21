/*
 * A202114  Numbers k such that 90*k + 53 is prime.
 * https://oeis.org/A202114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that 90*k + 53 is prime  (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

static idx_t *cache = NULL;      /* cached k-values */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap   = 0;    /* current allocation */

/* enlarge the cache so it can hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    idx_t *p = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    if (!p)
        exit(EXIT_FAILURE);   /* unrecoverable allocation failure */
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1, undefined for 0 */
        return 0;

    /* if the requested term is already cached, return it */
    if (cache_size >= (size_t)n)
        return (seq_t)cache[n - 1];

    /* start searching from the next candidate after the last cached value */
    idx_t k = cache_size ? cache[cache_size - 1] + 1 : 0;

    /* generate terms until we reach the n-th one */
    while (cache_size < (size_t)n)
    {
        /* compute 90*k + 53; use seq_t for the result to allow big integers */
        seq_t val = (seq_t)(90ULL * (uint64_t)k + 53ULL);

        if (isPrimeGeneric(val))
        {
            grow_cache(cache_size + 1);
            cache[cache_size] = k;
            cache_size++;
        }

        k++;
    }
    return (seq_t)cache[n - 1];
}
