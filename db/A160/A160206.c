/*
 * A160206  Positive numbers y such that y^2 is of the form x^2+(x+857)^2 with
 *          integer x.
 * https://oeis.org/A160206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 28, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160206
   a(n) = 6*a(n-3) - a(n-6)   for n > 6
   a(1)=697, a(2)=857, a(3)=1117, a(4)=3065, a(5)=4285, a(6)=6005
   The function caches computed values so that calls in any order remain
   fast.  It works for any seq_t (including arbitrary‑precision) that
   supports the usual integer operators. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset is 1 – a(0) is unused; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache    = NULL;   /* array holding a(1)…a(count) */
    static idx_t capacity  = 0;      /* allocated size of cache      */
    static idx_t count     = 0;      /* number of terms stored       */

    /* Helper: initial seed values for n = 1…6 */
    static const seq_t init[6] = {
        (seq_t)697, (seq_t)857, (seq_t)1117,
        (seq_t)3065, (seq_t)4285, (seq_t)6005
    };

    /* If we haven't allocated enough space, grow the array */
    if (n > capacity) {
        idx_t new_cap = n;                   /* at least n slots          */
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache)
            abort();                         /* allocation failed         */

        /* copy any existing values */
        for (idx_t i = 0; i < count; ++i)
            new_cache[i] = cache[i];

        /* free the old array */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif

        cache     = new_cache;
        capacity  = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = count; i < n; ++i) {     /* i = index in 0‑based array */
        idx_t k = i + 1;                    /* term number (1‑based)     */
        if (k <= 6) {
            cache[i] = init[k - 1];
        } else {
            /* recurrence: a(k) = 6*a(k-3) - a(k-6) */
            cache[i] = (seq_t)(6 * cache[i - 3] - cache[i - 6]);
        }
    }
    count = n;      /* store how many terms we have now */

    return cache[n - 1];
}
