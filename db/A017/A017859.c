/*
 * A017859  Expansion of 1/(1-x^7-x^8-x^9-x^10).
 * https://oeis.org/A017859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + a(n-8) + a(n-9) + a(n-10) for n>9 initial
 *            values: a(0)=1; a(1..6)=0; a(7..10)=1
 * Range:     u8 n <= 47, u16 n <= 80, u32 n <= 147, u64 n <= 281,
 *            u128 n <= 550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;   /* array of cached terms */
    static idx_t max_n = 0;       /* largest n that has been computed */
    static size_t cap = 0;        /* current capacity of cache */

    /* initialise cache on the first call */
    if (cache == NULL) {
        /* at least 11 slots to store a(0) … a(10) */
        cap = 11;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */

        /* initialise all entries to 0 */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;

        /* explicit initial values */
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        /* a(1) … a(6) are already 0 */
        cache[7] = (seq_t)1;   /* a(7) = 1 */
        cache[8] = (seq_t)1;   /* a(8) = 1 */
        cache[9] = (seq_t)1;   /* a(9) = 1 */
        cache[10] = (seq_t)1;  /* a(10) = 1 */
        max_n = 10;
    }

    /* extend cache if the requested n is beyond what we have computed */
    if (n > max_n) {
        /* new capacity needed */
        idx_t needed = n + 1;
        if ((size_t)needed > cap) {
            /* grow the allocation */
            size_t new_cap = (size_t)needed;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) return 0;  /* allocation failure */
            /* initialise the newly added entries to 0 */
            for (size_t i = cap; i < new_cap; ++i) new_cache[i] = 0;
            cache = new_cache;
            cap = new_cap;
        }

        /* compute terms from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = (seq_t)0;
            } else if (i <= 10) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i - 7] + cache[i - 8] + cache[i - 9] + cache[i - 10];
            }
        }
        max_n = n;
    }

    return cache[n];
}
