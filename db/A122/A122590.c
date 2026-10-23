/*
 * A122590  a(n) = 2*a(n-1) - a(n-2) - (a(n-1)^2 + a(n-2)^2).
 * https://oeis.org/A122590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - (a(n-1)^2 + a(n-2)^2), a(1)=a(2)=1
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 – safe placeholder */

#ifdef __cplusplus
    /* C++ build – use std::vector so arbitrary‑precision integer types are
       properly constructed. */
    static std::vector<seq_t> cache;
    static idx_t cache_max = 0;
    if (n > cache_max) {
        if (cache.size() < (size_t)n + 1)
            cache.resize((size_t)n + 1);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                cache[i] = (seq_t)1;
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t term = (seq_t)2 * a1 - a2;
                term -= a1 * a1;
                term -= a2 * a2;
                cache[i] = term;
            }
        }
        cache_max = n;
    }
    return cache[n];
#else
    /* C build – allocate with malloc/realloc; works for built‑in integer types. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;
    if (n > cache_max) {
        size_t new_size = (size_t)n + 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                cache[i] = (seq_t)1;
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t term = (seq_t)2 * a1 - a2;
                term -= a1 * a1;
                term -= a2 * a2;
                cache[i] = term;
            }
        }
        cache_max = n;
    }
    return cache[n];
#endif
}
