/*
 * A145183  Continued cotangent recurrence a(n+1)=a(n)^3+3*a(n) and a(1)=9.
 * https://oeis.org/A145183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(n+1)=a(n)^3+3*a(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n<1; return 0 as a safe default */
        return 0;

    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t  cache_cap = 0;      /* allocated capacity */
    static idx_t   cache_len = 0;      /* number of terms stored */

    /* initialise cache with a(1) if empty */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failed */
        cache[0] = (seq_t)9;                /* a(1) */
        cache_len = 1;
    }

    /* grow cache if necessary */
    if (n > cache_len) {
        idx_t needed = n;                   /* need indices 1..n inclusive   */
        if (needed > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                        needed * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failed */
            cache = tmp;
            cache_cap = needed;
        }

        /* compute missing terms from cache_len+1 to n */
        for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
            seq_t prev  = cache[idx - 2];    /* a(idx-1)  (idx-2 is 0‑based) */
            seq_t sq    = prev * prev;      /* a^2       */
            seq_t cube  = sq * prev;        /* a^3       */
            seq_t thr   = prev + prev + prev; /* 3*a     */
            seq_t next  = cube + thr;       /* a^3 + 3*a*/
            cache[idx - 1] = next;          /* store a(idx) */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
