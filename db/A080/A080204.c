/*
 * A080204  Number of fixed points under n-fold inflation for the substitution
 *          rule a->abc, b->ab, c->b that underlies the Kolakoski (3,1)
 *          sequence.
 * https://oeis.org/A080204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=10, a(4)=23; a(n) = 2*a(n-1) + a(n-3) + 2
 *            for n ≥ 5 (A080204)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑indexed cache for computed values */
static idx_t cache_cap = 0;   /* largest index stored in cache */

/* Return the n‑th term of A080204; n ≥ 1. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = (n > 4 ? n : 4);
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* OOM safeguard */

        /* Base values.  cache[0] is unused but keeps indices neat. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)23;
    }

    /* Extend cache if required. */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* OOM safeguard */
        cache = new_cache;

        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) + a(i-3) + 2 */
            cache[i] = (cache[i - 1] * 2) + cache[i - 3] + (seq_t)2;
        }
        cache_cap = n;
    }
    return cache[n];
}
