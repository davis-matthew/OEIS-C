/*
 * A027022  a(n) = Sum_{k=floor((n+1)/2)..n} T(k,n-k); i.e., a(n) is n-th
 *          diagonal sum of left-justified array T given by A027011.
 * https://oeis.org/A027022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + 3*(a(n-2)-a(n-4)) + (a(n-3)-a(n-5)), with
 *            initial values a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=11, a(6)=15
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 54, u64 n <= 110,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms   */
    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t cache_len = 0;   /* number of stored terms */

    /* base case values (a(1)..a(6)) */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – should be rare */

        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)3;  /* a(2) */
        cache[2] = (seq_t)4;  /* a(3) */
        cache[3] = (seq_t)7;  /* a(4) */
        cache[4] = (seq_t)11; /* a(5) */
        cache[5] = (seq_t)15; /* a(6) */
    }

    /* extend cache up to requested n */
    if (n > cache_len) {
        /* Reallocate cache to fit n terms */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;

        /* compute terms from current length up to n-1 (0‑based) */
        for (idx_t i = cache_len; i < n; ++i) {
            /* indices for recurrence:
               i-2  -> a(n-2)
               i-3  -> a(n-3)
               i-4  -> a(n-4)
               i-5  -> a(n-5)
               i-6  -> a(n-6)   */
            seq_t diff1 = cache[i-2] - cache[i-4]; /* a(n-2) - a(n-4)  */
            seq_t diff2 = cache[i-3] - cache[i-5]; /* a(n-3) - a(n-5)  */
            seq_t val   = cache[i-6];             /* a(n-6)           */

            val += (seq_t)3 * diff1;  /* add 3*(a(n-2)-a(n-4)) */
            val += diff2;             /* add (a(n-3)-a(n-5))   */

            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n-1];
}
