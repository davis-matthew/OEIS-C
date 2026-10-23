/*
 * A129640  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+313)^2 = y^2.
 * https://oeis.org/A129640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 626 for n > 6 with initial values
 *            a(1)=0, a(2)=155, a(3)=464, a(4)=939, a(5)=1764, a(6)=3515
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values (1‑based indexing). */
    static seq_t *cache   = NULL;
    static size_t cache_cap   = 0;   /* allocated size in elements */
    static idx_t  computed_up_to = 0;/* largest k with cache[k] valid */
    static int    initialized    = 0;/* 0: base cases not set yet */

    /* Ensure the array is large enough for all indices up to n.
       The base cases require at least indices 1…6, so we ensure
       capacity for max(n,6) + 1 (extra element for 1‑based indexing). */
    size_t required = (size_t)(( n > 6 ) ? n : 6 ) + 1U;   /* n or 6, plus 1 */
    if (required > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            required * sizeof(seq_t));
        if (!new_cache)                     /* allocation failed – fall back */
            return (seq_t)0;
        /* initialize newly allocated elements; old part already holds values */
        for (size_t i = cache_cap; i < required; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = required;
    }

    /* Set the initial values once.  The indices 1…6 are used. */
    if (!initialized) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)155;
        cache[3] = (seq_t)464;
        cache[4] = (seq_t)939;
        cache[5] = (seq_t)1764;
        cache[6] = (seq_t)3515;
        computed_up_to = 6;
        initialized = 1;
    }

    /* If the requested value has already been computed, simply return it. */
    if (n <= computed_up_to)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t value = cache[i - 3];
        value = value * (seq_t)6;          /* 6*a(i-3) */
        value = value - cache[i - 6];      /* - a(i-6) */
        value = value + (seq_t)626;        /* + 626 */
        cache[i] = value;
    }
    computed_up_to = n;
    return cache[n];
}
