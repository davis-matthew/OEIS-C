/*
 * A201601  Primes of the form 6n^2 + 7.
 * https://oeis.org/A201601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 4308, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 6*n^2 + 7 (offset 1) */
/* The function generates primes of this form by brute‑force testing
   numbers 6*k^2 + 7, caching the results for fast look‑up.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first prime of the form 6*n^2+7. */
    if (n == 0)            /* behaviour below offset is irrelevant */
        return 0;

    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t k = 0;                /* next k to test */

    /* Generate until we have at least n values cached. */
    while (cache_cnt < n) {
        seq_t val;
        /* Compute 6*k^2 + 7 in a wider integer to avoid overflow of
           intermediate products, then cast down to seq_t. */
        uint64_t tmp = 6ULL * (uint64_t)k * (uint64_t)k + 7ULL;
        val = (seq_t)tmp;
        ++k;

        /* Test primality.  The utility isPrimeGeneric works for all
           supported seq_t sizes, including big integers. */
        if (!isPrimeGeneric(val))
            continue;          /* skip composite values */

        /* Ensure the cache has room. */
        if (cache_cnt == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp_ptr)
                abort();                   /* Out of memory. */
            cache = tmp_ptr;
            cache_cap = new_cap;
        }
        cache[cache_cnt++] = val;
    }

    return cache[n - 1];
}
