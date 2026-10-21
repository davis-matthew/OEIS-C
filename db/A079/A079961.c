/*
 * A079961  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={1,4}.
 * https://oeis.org/A079961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-4)+a(n-6), a(0)=1; (OEIS A079961)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the number of compositions of n using parts {1,3,4,6}.  This is a
   linear recurrence; we memoise the values in a static array so that each
   value is computed only once regardless of call order. */
seq_t a(idx_t n)
{
    /* Static memoisation table: cache[i] holds a(i) for i < cache_len. 
       cache_cap is the allocated capacity. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise: first term a(0)=1. */
    if (cache_len == 0) {
        cache_cap = 16;                /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – return 0 */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Grow the table if necessary. */
    if (n >= cache_len) {
        /* Increase allocation until we can hold index n. */
        if (cache_cap <= n) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 16;
            while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;         /* allocation failure */
            cache_cap = new_cap;
        }

        /* Compute new terms up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 1) val += cache[i - 1];
            if (i >= 3) val += cache[i - 3];
            if (i >= 4) val += cache[i - 4];
            if (i >= 6) val += cache[i - 6];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
