/*
 * A077889  Expansion of 1/( (1-x)*(1 + x^2 + x^3) ).
 * https://oeis.org/A077889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-4), a(0)=1, a(1)=1, a(2)=0, a(3)=-1
 * Range:     i8 n <= 29, i16 n <= 56, i32 n <= 114, i64 n <= 232,
 *            i128 n <= 464, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached computed values to allow non‑sequential calls. */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* number of elements stored (0 … cached_size-1) */

    if (cached_size <= n) {
        /* Grow the cache to include a[n] */
        idx_t new_size = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        /* copy old values */
        for (idx_t i = 0; i < cached_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
#endif
        cache = new_cache;

        /* Compute new values sequentially */
        for (idx_t i = cached_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }
            seq_t val = cache[i - 1];          /* a(i-1) always available */
            if (i >= 2) val -= cache[i - 2];   /* a(i-2) if i≥2, else 0 */
            if (i >= 4) val += cache[i - 4];   /* a(i-4) if i≥4, else 0 */
            cache[i] = val;
        }
        cached_size = new_size;
    }
    return cache[n];
}
