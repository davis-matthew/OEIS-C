/*
 * A094790  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 7 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2*n, s(0) = 1, s(2n) = 3.
 * https://oeis.org/A094790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) Initial values: a(1) = 1,
 *            a(2) = 3, a(3) = 9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle offset: a(1) is first term */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Lazy static cache for arbitrary n */
    static seq_t *cache = NULL;   /* cache[0] unused */
    static idx_t cache_len = 0;   /* highest index computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 3;                     /* we precompute up to n=3 */
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure, unlikely */
        cache[0] = (seq_t)0;   /* unused slot */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)3;   /* a(2) */
        cache[3] = (seq_t)9;   /* a(3) */
    }

    /* Extend cache if needed */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* allocation failure, unlikely */
        cache = new_cache;
        /* Compute new terms using recurrence */
        idx_t i;
        for (i = cache_len + 1; i <= n; ++i) {
            cache[i] = (5 * cache[i - 1]) - (6 * cache[i - 2]) + cache[i - 3];
        }
        cache_len = n;
    }

    return cache[n];
}
