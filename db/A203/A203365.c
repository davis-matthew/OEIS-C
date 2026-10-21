/*
 * A203365  Number of n X 2 0..2 arrays with every 1 immediately preceded by 0
 *          to the left or above, and every 2 immediately preceded by both a 1
 *          and a 0.
 * https://oeis.org/A203365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) + 2*a(n-3) + a(n-4) (n >= 5) with
 *            initial values: a(1) = 2 , a(2) = 9 , a(3) = 30 , a(4) = 110
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for memoised values.  cache[i] holds a(i). */
static seq_t *cache = NULL;      /* pointer to allocated array */
static size_t cache_cap = 0;     /* allocated size (elements) */
static size_t cache_len = 0;     /* next index to compute */

/* Grow the cache to at least `new_cap` elements, preserving existing data. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort the program (no partial results). */
        abort();
    }
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Expand the cache if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        grow_cache(new_cap);
    }

    /* Compute any missing values using the recurrence. */
    while (cache_len <= (size_t)n) {
        /* For indices 1..4 the values are pre‑initialised, so the loop only
           runs for k >= 5.  The recurrence uses a(k-1)..a(k-4). */
        cache[cache_len] = 2 * cache[cache_len - 1]
                         + 5 * cache[cache_len - 2]
                         + 2 * cache[cache_len - 3]
                         +     cache[cache_len - 4];
        cache_len++;
    }
}

/* Return the n-th term of the sequence (offset 1).  n >= 1.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* behaviour for n < 1 is undefined */

    /* Initialise the cache on the very first call. */
    if (!cache) {
        grow_cache(8);
        cache[0] = (seq_t)0;           /* unused, but keeps indices aligned */
        cache[1] = (seq_t)2;           /* a(1)  */
        cache[2] = (seq_t)9;           /* a(2)  */
        cache[3] = (seq_t)30;          /* a(3)  */
        cache[4] = (seq_t)110;         /* a(4)  */
        cache_len = 5;                 /* next index to compute */
    }

    if ((size_t)n >= cache_len)
        ensure_cache(n);

    return cache[n];
}
