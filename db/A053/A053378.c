/*
 * A053378  a(n) contains n digits (either '5' or '8') and is divisible by
 *          2^n.
 * https://oeis.org/A053378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1) * ( (a(n-1)/2^(n-1) mod 2 == 0) ? 8 : 5
 *            ) a(1) = 8 Definition: a(n) is the n‑digit number consisting
 *            only of 5 or 8 that is divisible by 2^n. This implementation
 *            builds the sequence iteratively and memoises the results in a
 *            static array so that calls in any order are O(1)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_seq_array, div_by_pow2, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/div_by_pow2.h"     /* int div_by_pow2(seq_t x, idx_t k) */
#include "../utils/alloc_seq_array.h"/* seq_t *alloc_seq_array(size_t n) */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *p) */

/* Keep a cache of computed terms and the current power of 10 value */
static seq_t *cache = NULL;     /* array of a(i), zero‑based */
static idx_t cache_size = 0;    /* number of terms stored in cache */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static seq_t pow10_cur = (seq_t)1; /* 10^(current index) , initially 10^0 */

/* Expand the cache to hold at least n+1 entries (0‑based) */
static void grow_cache(idx_t n)
{
    if (cache_cap >= (size_t)n + 1)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                 /* double until large enough */

    seq_t *new_tbl = alloc_seq_array(new_cap);
    if (cache) {
        memcpy(new_tbl, cache, cache_size * sizeof(seq_t));
        free_seq_array(cache);
    }
    cache      = new_tbl;
    cache_cap  = new_cap;
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1 – behaviour below does not matter */
        return (seq_t)0;

    /* Grow cache up to requested n if necessary */
    if (cache_size < (size_t)n) {
        grow_cache(n);
        /* Initialise the first term if cache was empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)8;    /* a(1) */
            pow10_cur = (seq_t)1;   /* 10^0 */
            cache_size = 1;
        }

        /* Generate any missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* Multiply current power of 10 by 10 to get 10^(i-1) */
            pow10_cur = pow10_cur * (seq_t)10;

            seq_t prev = cache[i - 2];                 /* a(i-1) */
            seq_t q    = div_by_pow2(prev, i - 1);    /* (a(i-1) / 2^(i-1)) */
            bool odd   = (q & (seq_t)1) != 0;
            seq_t digit = odd ? (seq_t)5 : (seq_t)8;
            cache[i - 1] = prev + pow10_cur * digit;
        }
        cache_size = n;
    }

    return cache[n - 1];   /* zero‑based index */
}
