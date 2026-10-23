/*
 * A041364  Numerators of continued fraction convergents to sqrt(197).
 * https://oeis.org/A041364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 28*a(n-1) + a(n-2), a(0)=14, a(1)=393
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache    = NULL;   /* array of values */
    static idx_t  cache_cnt = 0;     /* number of terms currently in cache */

    /* initialise cache on the first call or enlarge if needed */
    if (cache == NULL) {
        idx_t init_len = (n > 1) ? (n + 1) : 2;      /* at least two slots */
        cache = (seq_t *)malloc((size_t)init_len * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failure */

        cache[0] = 14;
        cache[1] = 393;
        cache_cnt = 2;
    } else if (cache_cnt <= n) {
        /* enlarge the array to hold indices 0..n */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        /* no need to initialise the new slots explicitly because we
           compute them immediately below */
        cache_cnt = cache_cnt; /* unchanged until we compute the new terms */
    }

    /* fast‑path for already computed terms */
    if (n < cache_cnt) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_cnt; i <= n; ++i) {
        cache[i] = (seq_t)28 * cache[i - 1] + cache[i - 2];
    }

    cache_cnt = n + 1;
    return cache[n];
}
