/*
 * A127389  Number of fixed points in range [A014137(n-1)..A014138(n-1)] of
 *          permutations A127377/A127378 and A127387.
 * https://oeis.org/A127389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   (n+1) * a(n) = (n-2) * a(n-1) + (5*n-7) * a(n-2)
                   + 3*(n-2) * a(n-3)
                   - 4*(n-5) * (a(n-4) + a(n-5))
   with a(0)=1, a(1)=1, a(2)=0, a(3)=1, a(4)=2. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initial terms for the recurrence */
    static const seq_t init[5] = {1, 1, 0, 1, 2};

    /* ensure cache is big enough and filled with initial values */
    if (cache_len == 0 || cache_cap < 5) {
        cache_cap = 64;                 /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);            /* allocation failure */
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term;
        if (i < 5) {
            /* initial values already stored */
            term = cache[i];
        } else {
            seq_t t1 = (seq_t)(i - 2) * cache[i - 1];
            seq_t t2 = (seq_t)(5 * i - 7) * cache[i - 2];
            seq_t t3 = (seq_t)(3 * (i - 2)) * cache[i - 3];
            seq_t t4 = (seq_t)(4 * (i - 5))
                        * (cache[i - 4] + cache[i - 5]);

            /* numerator = t1 + t2 + t3 - t4 */
            seq_t num = t1;
            num += t2;
            num += t3;
            /* subtraction is safe: recurrence guarantees non‑negative result */
            num -= t4;

            term = num / (i + 1);
        }
        cache[i] = term;
    }
    cache_len = n + 1;

    return cache[n];
}
