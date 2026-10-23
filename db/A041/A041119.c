/*
 * A041119  Denominators of continued fraction convergents to sqrt(68).
 * https://oeis.org/A041119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   a(0)=1, a(1)=4, a(2)=65, a(3)=264, * a(n)=66*a(n-2)-a(n-4) for
 *            n≥4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array storing computed terms      */
    static size_t cache_cap = 0;    /* allocated capacity                 */
    static size_t cache_size = 0;   /* number of terms currently stored   */

    /* Grow the cache if we need to reach index n. */
    if (n >= cache_size) {
        size_t need = (size_t)n + 1;                 /* required capacity      */
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Compute the new terms using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)        cache[i] = (seq_t)1;
            else if (i == 1)   cache[i] = (seq_t)4;
            else if (i == 2)   cache[i] = (seq_t)65;
            else if (i == 3)   cache[i] = (seq_t)264;
            else
                cache[i] = (seq_t)((seq_t)66 * cache[i-2] - cache[i-4]);
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
}
