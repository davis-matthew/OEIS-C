/*
 * A135908  Clique number of commuting graph of symmetric group S_n.
 * https://oeis.org/A135908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) for n > 7 a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=11
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary‑precision or small integer types */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;
    static idx_t  cache_max  = 0;   /* highest index computed and stored */

    /* initialise cache with the first eight terms */
    if (cache == NULL) {
        cache_cap = 8;                     /* store indices 0 … 7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed */
        /* zero‑initialise and then set the known values */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 2;
        cache[4] = 3;
        cache[5] = 5;
        cache[6] = 8;
        cache[7] = 11;
        cache_max = 7;                     /* highest stored index */
    }

    /* enlarge the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        /* zero any new entries so that garbage is not used later */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values using the recurrence only for n > 7 */
    if (n > cache_max) {
        idx_t start = (cache_max < 7) ? 8 : cache_max + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = cache[i-1];         /* a(i-1)      */
            val += 3 * cache[i-3];          /* + 3*a(i-3)  */
            val -= 3 * cache[i-4];          /* - 3*a(i-4)  */
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
