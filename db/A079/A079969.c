/*
 * A079969  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={2}.
 * https://oeis.org/A079969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079969 – number of compositions of n using parts {1,2,4,5,6}
   Recurrence: a(0)=1,
               a(n)=a(n−1)+a(n−2)+a(n−4)+a(n−5)+a(n−6)   for n≥1
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6, … */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cache_size = 0; /* number of terms cached (a(0) … a(cache_size-1)) */

    /* initialise cache with a(0) if this is the first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_size = 1;
    }

    /* expand and compute up to a(n) if necessary */
    size_t needed = (size_t)n + 1;
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        /* compute terms that are not yet in the cache */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t sum = (seq_t)0;
            if (i >= 1) sum += cache[i - 1];
            if (i >= 2) sum += cache[i - 2];
            if (i >= 4) sum += cache[i - 4];
            if (i >= 5) sum += cache[i - 5];
            if (i >= 6) sum += cache[i - 6];
            cache[i] = sum;
        }
        cache_size = needed;
    }

    return cache[(size_t)n];
}
