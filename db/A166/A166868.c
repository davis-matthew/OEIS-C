/*
 * A166868  Convolution of Jacobsthal(n+2) and Pell(n+1).
 * https://oeis.org/A166868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 5*a(n-3) - 2*a(n-4), * initial
 *            values: a(0)=1, a(1)=5, a(2)=16, a(3)=48
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A166868: convolution of Jacobsthal(n+2) and Pell(n+1). */

#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* pointer to array */
    static size_t cache_len = 0;     /* current number of stored terms */

    /* initialise_cache */
    if (cache == NULL) {
        cache     = alloc_seq_array(4);  /* allocate space for 4 terms */
        cache_len = 4;
        cache[0] = 1;   /* a(0) */
        cache[1] = 5;   /* a(1) */
        cache[2] = 16;  /* a(2) */
        cache[3] = 48;  /* a(3) */
    }

    /* if n is already in cache just return */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* extend cache to accommodate n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    grow_seq_array(&cache, &cache_len, new_len); /* now cache_len == new_len */

    /* compute missing terms using the recurrence */
    for (size_t i = old_len; i < cache_len; ++i) {
        /* a(i) = 3*a(i-1) + a(i-2) - 5*a(i-3) - 2*a(i-4) */
        seq_t t1 = 3 * cache[i - 1] + cache[i - 2];
        seq_t t2 = 5 * cache[i - 3] + 2 * cache[i - 4];
        cache[i] = t1 - t2;   /* result is known to be non‑negative */
    }

    return cache[(size_t)n];
}
