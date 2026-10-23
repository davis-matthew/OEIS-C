/*
 * A038367  Numbers n with property that (product of digits of n) is divisible
 *          by (sum of digits of n).
 * https://oeis.org/A038367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038367: numbers n whose product of digits is divisible by the sum of digits.
   The function generates the sequence by incremental search. */

/*  Include helpers for digit sum and product.  */
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"

#include <stdlib.h>

/* Static cache for previously computed terms.  */
static seq_t *cache = NULL;    /* array of sequence values */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Next integer to test for the property.  */
static seq_t next_check = 0;

/* Ensure the cache has enough capacity for at least one more element.  */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap * 2);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* Out of memory: unlikely for the target sizes. */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Append a new term to the cache.  */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap)
        grow_cache();
    cache[cache_len++] = val;
}

/* Main function: return the nth term (1‑based).  */
seq_t a(idx_t n)
{
    /* Lazily initialize the search start at 1.  */
    if (next_check == 0)
        next_check = 1;

    /* Generate terms until we have reached the requested index.  */
    while (cache_len < (size_t)n) {
        seq_t sum   = digit_sum(next_check);
        seq_t prod  = digit_product(next_check);

        /* Divisibility test: prod % sum == 0.  */
        if (sum != 0 && (prod % sum) == 0)
            cache_append(next_check);

        /* Advance to the next integer.  */
        next_check++;
    }

    /* Return the n‑th term (1‑based).  */
    return cache[(size_t)n - 1];
}
