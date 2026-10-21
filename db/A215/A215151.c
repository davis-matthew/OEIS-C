/*
 * A215151  a(1) = 1; thereafter a(n+1) = 0 if bigomega(n) =
 *          bigomega(2^a(n)+n), otherwise a(n+1) = a(n)+1.
 * https://oeis.org/A215151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(1) = 1; thereafter * a(n+1) = 0 if bigomega(n) =
 *            bigomega(2^a(n)+n), * otherwise a(n+1) = a(n)+1. Uses the helper
 *            bigomega and pow2
 * Range:     u8 n <= 20, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   bigomega, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/bigomega.h"
#include "../utils/pow2.h"

/* static cache for memoised values */
static seq_t *cache = NULL;     /* array of a(i) values, 0‑based */
static size_t cache_len = 0;    /* number of elements stored */
static size_t cache_cap = 0;    /* capacity of the array */

/* grow the cache so that it can hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* in a contest setting, aborting is acceptable; real code should handle allocation error */
        exit(EXIT_FAILURE);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is defined only for n >= 1; treat as 0 */
        return 0;

    /* ensure cache includes a(1) */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    if (n <= (idx_t)cache_len)
        return cache[n-1];      /* cached result */

    /* compute values up to n */
    for (idx_t k = (idx_t)cache_len + 1; k <= n; ++k) {
        seq_t prev = cache[k-2];          /* a(k-1) */
        seq_t bigomega_km1 = bigomega((idx_t)(k-1));

        /* compute 2^a(k-1) */
        seq_t pow2val = pow2((idx_t)prev);

        /* sum = 2^a(k-1) + (k-1) */
        seq_t sum = pow2val + (seq_t)(k-1);

        seq_t bigomega_sum = bigomega((idx_t)sum);

        seq_t next;
        if (bigomega_km1 == bigomega_sum)
            next = 0;
        else
            next = prev + 1;

        grow_cache((size_t)k);
        cache[k-1] = next;
    }

    cache_len = (size_t)n;
    return cache[n-1];
}
