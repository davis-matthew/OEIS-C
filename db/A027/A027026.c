/*
 * A027026  a(n) = T(n,n+4), T given by A027023.
 * https://oeis.org/A027026
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000213(n+4) – 2*n*(n+3) for n>3 Equivalent linear
 *            recurrence (order 6): a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) -
 *            a(n-4) + 2*a(n-5) - a(n-6) for all n ≥ 10. Initial values:
 *            a(4)=1, a(5)=25, a(6)=85, a(7)=215, a(8)=477, a(9)=985. (For n<4
 *            the function returns 0, behaviour is irrelevant.)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 70, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour is unimportant. */
    if (n < 4u) {
        return (seq_t)0;
    }

    /* Static cache for memoised terms */
    static seq_t *cache = NULL;   /* array holding computed a(k) */
    static idx_t cap   = 0;       /* allocated capacity (size of cache) */
    static idx_t fill  = 0;       /* largest index computed so far (0‑based) */

    /* Ensure we have capacity up to n */
    if (n + 1u > cap) {
        /* Reallocate or allocate new array */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1u) * sizeof(seq_t));
        if (!new_cache) {            /* malloc failure – return 0 */
            return (seq_t)0;
        }
        cache = new_cache;
        cap   = n + 1u;
    }

    /* Initialise known terms that are not yet initialised in the cache. */
    /* We only need to initialise indices 4 through 9. */
    if (fill < 4u) {
        /* Guarantee indices 4..9 exist in the cache */
        if (cap < 10u) {
            /* Should never happen because we expanded to n+1 and n≥9 at least for the call. */
            const idx_t need = (n + 1u > 10u) ? n + 1u : 10u;
            seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cap   = need;
        }
        /* Initial constants for 4..9 */
        static const seq_t init_vals[10] = {
            (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
            (seq_t)1,  (seq_t)25, (seq_t)85,
            (seq_t)215,(seq_t)477,(seq_t)985
        };
        for (idx_t i = 4u; i <= 9u && i < cap; ++i) {
            cache[i] = init_vals[i];
        }
        fill = 9u;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = fill + 1u; i <= n; ++i) {
        if (i <= 9u) {
            /* Already initialised above, skip. */
            continue;
        }
        /* a(i) = 4*a(i-1) - 5*a(i-2) + 2*a(i-3) - a(i-4) + 2*a(i-5) - a(i-6) */
        seq_t term = (seq_t)4 * cache[i - 1u]
                   - (seq_t)5 * cache[i - 2u]
                   + (seq_t)2 * cache[i - 3u]
                   - cache[i - 4u]
                   + (seq_t)2 * cache[i - 5u]
                   - cache[i - 6u];
        cache[i] = term;
    }
    fill = (fill > n) ? fill : n;

    return cache[n];
}
