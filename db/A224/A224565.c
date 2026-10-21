/*
 * A224565  Number of (n+3) X 8 0..1 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 4*a(n-4) - 3*a(n-5) * +
 *            2*a(n-7) - a(n-8) for n > 10. * Initial values (a(1) … a(10)): *
 *            990, 611, 728, 861, 974, 1233, 1614, 2099, 2665, 3459
 * Range:     u8 none, u16 n <= 19, u32 n <= 54, u64 n <= 122, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, size_t */

static seq_t *cache = NULL;   /* cache for computed terms */
static size_t cache_len = 0;  /* number of terms stored in cache */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* initial 10 terms of the sequence (1‑indexed) */
static const seq_t init[10] = {
    990, 611, 728, 861, 974,
    1233, 1614, 2099, 2665, 3459
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n==0 */
    size_t want = (size_t)n;

    /* grow cache if necessary */
    if (want > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap < want) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i < want; ++i) {
        seq_t val;
        if (i < 10) {                     /* use hard‑coded terms */
            val = init[i];
        } else {
            /* recurrence: a(n) = 3*a(n-1)-2*a(n-2)-2*a(n-3)+4*a(n-4)
             *           -3*a(n-5)+2*a(n-7)-a(n-8)     (n = i+1)
             */
            seq_t s1 = 3 * cache[i-1];               /* 3*a(n-1) */
            s1 += 4 * cache[i-4];                    /* +4*a(n-4) */
            s1 += 2 * cache[i-7];                    /* +2*a(n-7) */

            seq_t s2 = 2 * cache[i-2];               /* 2*a(n-2) */
            s2 += 2 * cache[i-3];                    /* +2*a(n-3) */
            s2 += 3 * cache[i-5];                    /* +3*a(n-5) */
            s2 += cache[i-8];                        /* +   a(n-8) */

            val = s1 - s2;                           /* subtract negatives */
        }
        cache[i] = val;
    }

    cache_len = want;
    return cache[want - 1];
}
