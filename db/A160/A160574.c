/*
 * A160574  Positive numbers y such that y^2 is of the form x^2+(x+313)^2 with
 *          integer x.
 * https://oeis.org/A160574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6 with a(1)=233, a(2)=313,
 *            a(3)=493, a(4)=905, a(5)=1565, a(6)=2725 Indices start at 1;
 *            a(0) is unused and returns 0
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside the defined range */

    /* Static cache for previously computed terms (1‑indexed). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of allocated slots (>= n+1) */

    /* Grow the cache if needed. */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;          /* +1 for the unused 0 slot */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed: return 0 as sentinel */
        cache = tmp;
        /* initialize new entries to 0 (uncomputed). */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* Return value if already computed. */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Compute all terms up to n, filling the cache. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already known */

        seq_t val;
        switch (i) {
            case 1: val = 233; break;
            case 2: val = 313; break;
            case 3: val = 493; break;
            case 4: val = 905; break;
            case 5: val = 1565; break;
            case 6: val = 2725; break;
            default:
                /* a(i) = 6*a(i-3) - a(i-6)  for i > 6 */
                val = (seq_t)6 * cache[i - 3] - cache[i - 6];
                break;
        }
        cache[i] = val;
    }

    return cache[n];
}
