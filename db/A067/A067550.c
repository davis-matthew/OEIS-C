/*
 * A067550  a(n) = (n-1)!(n+2)!/(3*2^n).
 * https://oeis.org/A067550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)!(n+2)!/(3*2^n) (OEIS A067550)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1;  a(i)=a(i-1)*(i-1)(i+2)/2 for i≥2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] unused; cache[1] = a(1) = 1 */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* largest n computed and stored */

    if (n == 0)
        return 0;                  /* sequence is defined for n≥1 */

    /* first invocation: create cache for a(1) */
    if (cache == NULL) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));  /* indices 0..1 */
        if (!cache) return 0;                       /* allocation failed */
        cache[1] = (seq_t)1;
        max_cached = 1;
    }

    /* extend cache if needed */
    if (n > max_cached) {
        seq_t *newcache = (seq_t *)realloc(cache, (n + 2) * sizeof(seq_t));
        if (!newcache) return 0;                   /* allocation failed */
        cache = newcache;

        /* compute missing terms */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* factor = (i-1)(i+2)/2; use 128‑bit intermediate to avoid
               overflow for 64‑bit builds when possible. */
            unsigned __int128 mult = (unsigned __int128)(i - 1) * (i + 2);
            mult /= 2;
            cache[i] = cache[i - 1] * (seq_t)mult;
        }
        max_cached = n;
    }

    return cache[n];
}
