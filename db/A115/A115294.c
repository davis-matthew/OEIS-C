/*
 * A115294  Diagonal sums of correlation triangle for (1+x)^3/(1-x).
 * https://oeis.org/A115294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A115294: Diagonal sums of correlation triangle for (1+x)^3/(1-x)
 *
 *  Recurrence (derived from the rational generating function)
 *      a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + d(n),
 *  where     d(0)=1, d(1)=2, d(2)=4, d(3)=6, d(4)=6, d(5)=6,
 *           d(6)=4, d(7)=2, d(8)=1  and d(n)=0 for n>8.
 *
 *  Initial terms: a(0)=1, a(1)=4, a(2)=11, a(3)=25, a(4)=47.
 *
 *  The sequence satisfies this linear recurrence for all n ≥ 5.
 *  It grows polynomially (quadratically), so the recurrence permits
 *  fast O(n) computation with minimal memory.  Values are stored in a
 *  lazy‑computed cache so that successive calls with arbitrary n are
 *  handled efficiently regardless of call order.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constant offset array d(n) for 0≤n≤8 */
    static const seq_t d[9] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)6, (seq_t)6,
                                (seq_t)6, (seq_t)4, (seq_t)2, (seq_t)1 };

    /* static cache for computed values */
    static seq_t *cache      = NULL;   /* pointer to cache array */
    static idx_t cached_n    = 0;      /* highest index that has been computed */
    static size_t cache_sz   = 0;      /* allocated array size */

    /* ---------- allocation & initialisation ---------- */
    if (cache == NULL) {
        /* guarantee space for the first five terms, plus requested n */
        cache_sz = (size_t)(n + 1);
        if (cache_sz < 5) cache_sz = 5;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */

        /* set the first five terms explicitly */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)47;

        /* if the requested n is <5, we are done */
        if (n < 5) {
            cached_n = 4;            /* we have computed up to index 4 */
            return cache[n];
        }

        /* compute terms from 5 up to n using the recurrence */
        for (idx_t k = 5; k <= n; ++k) {
            seq_t val = (seq_t)2 * cache[k-1];
            val -= cache[k-2];
            val += cache[k-3];
            val -= (seq_t)2 * cache[k-4];
            val += cache[k-5];
            if (k <= 8) val += d[k];   /* d(k) = 0 for k>8 */
            cache[k] = val;
        }

        cached_n = n;   /* last computed index */
        return cache[n];
    }

    /* ---------- cache extension if needed ---------- */
    if ((size_t)(n + 1) > cache_sz) {          /* grow array */
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failure */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* ---------- compute missing terms ---------- */
    if (n > cached_n) {
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            seq_t val = (seq_t)2 * cache[k-1];
            val -= cache[k-2];
            val += cache[k-3];
            val -= (seq_t)2 * cache[k-4];
            val += cache[k-5];
            if (k <= 8) val += d[k];           /* add the offset if applicable */
            cache[k] = val;
        }
        cached_n = n;
    }

    return cache[n];
}
