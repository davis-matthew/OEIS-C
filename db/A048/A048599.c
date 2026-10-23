/*
 * A048599  Partial products of the sequence (A001097) of twin primes.
 * https://oeis.org/A048599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial products of the twin‑prime sequence (A048599).  The nth term
   a(n) is the product of the first n primes that belong to a twin prime
   pair (p, p+2).  a(0)=1 according to the empty product convention.  A
   lazy, memoised algorithm is used: the cache of already‑computed terms
   is extended on demand.  The function uses the helper
   bool is_prime(idx_t p) from the utils library. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /*---- Cached sequence values -------------------------------------------------*/
    static seq_t *cache      = NULL;   /* array of a(i) with i < cache_size      */
    static size_t cache_cap  = 0;      /* allocated capacity (in elements)       */
    static size_t cache_size = 0;      /* number of terms actually cached        */

    /*---- State for generating further twin primes --------------------------------*/
    static seq_t  product      = 1;     /* running product of twin‑prime primes   */
    static idx_t  next_candidate = 3;   /* odd integer to test for primality      */

    /* Initialise cache on first entry: a(0)=1 */
    if (cache == NULL) {
        cache_cap   = 8;                /* small initial capacity                 */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure → return 0          */
        cache[0]    = 1;                /* a(0)                                 */
        cache_size  = 1;                /* one value cached                      */
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1UL : 8;
        while (new_cap <= (size_t)n + 1U)
            new_cap <<= 1UL;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;     /* allocation failure */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache       = new_cache;
        cache_cap   = new_cap;
    }

    /* Extend the cache until it contains a(n) */
    while (cache_size <= (size_t)n) {
        /* Skip to the next odd integer that is prime */
        while (!is_prime(next_candidate))
            next_candidate += 2UL;

        /* Determine whether this prime is part of a twin pair */
        bool has_twin = false;
        if (next_candidate > 2 && is_prime(next_candidate - 2))
            has_twin = true;
        else if (is_prime(next_candidate + 2))
            has_twin = true;

        if (has_twin) {
            /* Multiply into the running product and store the new term */
            product = product * (seq_t)next_candidate;
            cache[cache_size] = product;
            ++cache_size;
        }

        /* Advance to the next odd candidate */
        next_candidate += 2UL;
    }

    return cache[(size_t)n];
}
