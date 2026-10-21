/*
 * A082585  a(1)=1, a(n) = ceiling(r(5)*a(n-1)) where r(5) = (1/2)*(5 +
 *          sqrt(29)) is the positive root of X^2 = 5*X + 1.
 * https://oeis.org/A082585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082585: a(1)=1, a(2)=6, a(3)=32,
   a(n)=6*a(n-1)-4*a(n-2)-a(n-3) for n>3
   (equivalent to a(n)=ceil(r5*a(n-1)) where r5=(5+√29)/2) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated size (cache_len-1 is max index used) */
    static idx_t computed = 0;    /* highest index already computed */

    /* initialise cache and base values on first call */
    if (cache_len == 0) {
        cache_len = 6; /* at least indices 1..5 needed on first expansion */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* base values (indices 1..3) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)32;
        computed = 3;
    }

    /* if requested n is already available, return it */
    if (n <= computed) return cache[n];

    /* extend cache to hold up to n */
    if (n + 1 > cache_len) {          /* +1 to keep 1-indexed array */
        idx_t new_len = (n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute new entries using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 6*a(i-1) - 4*a(i-2) - a(i-3) */
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        cache[i] = (seq_t)(6 * t1 - 4 * t2 - t3);
    }
    computed = n;
    return cache[n];
}
