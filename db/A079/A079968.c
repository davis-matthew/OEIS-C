/*
 * A079968  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={3}.
 * https://oeis.org/A079968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-5)+a(n-6) * Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=14, a(6)=27
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of elements allocated */

    /* initialise cache with first 7 terms (indices 0..6) */
    if (cache == NULL) {
        cache_len = 7; /* indices 0..6 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0; /* malloc failed – return zero */

        cache[0] = 1; /* a(0) */
        cache[1] = 1; /* a(1) */
        cache[2] = 2; /* a(2) */
        cache[3] = 4; /* a(3) */
        cache[4] = 7; /* a(4) */
        cache[5] = 14;/* a(5) */
        cache[6] = 27;/* a(6) */
    }

    /* extend the cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* realloc failed – return zero */
        cache = new_cache;

        /* compute new terms using recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            if (i >= 1) sum += cache[i - 1];
            if (i >= 2) sum += cache[i - 2];
            if (i >= 3) sum += cache[i - 3];
            if (i >= 5) sum += cache[i - 5];
            if (i >= 6) sum += cache[i - 6];
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
