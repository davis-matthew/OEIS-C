/*
 * A056527  Numbers where iterated sum of digits of square settles down to a
 *          cyclic pattern (in fact 13, 16, 13, 16, ...).
 * https://oeis.org/A056527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=2, a(2)=4, a(3)=5, a(4)=7, a(5)=11 a(n) = a(n-1) + a(n-4) -
 *            a(n-5) for n>5 (zero‑based recurrence: cache[i] = cache[i-1] +
 *            cache[i-4] - cache[i-5] for i>=5)
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored, i.e. cache indices 0..cache_len-1 */

    if (n == 0) return 0; /* undefined, but harmless */

    /* Grow the cache if necessary */
    if (cache_len < n) {
        size_t new_len = (size_t)n; /* new number of terms to store */
        /* realloc returns void* in C++, cast explicitly to satisfy both C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;

        /* Compute new terms sequentially */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0)       cache[i] = (seq_t)2;
            else if (i == 1)  cache[i] = (seq_t)4;
            else if (i == 2)  cache[i] = (seq_t)5;
            else if (i == 3)  cache[i] = (seq_t)7;
            else if (i == 4)  cache[i] = (seq_t)11;
            else              cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
        }

        cache_len = n;
    }

    return cache[n-1]; /* a(n) corresponds to cache index n-1 */
}
