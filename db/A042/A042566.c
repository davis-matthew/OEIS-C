/*
 * A042566  Numerators of continued fraction convergents to sqrt(812).
 * https://oeis.org/A042566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042566: Numerators of continued fraction convergents to sqrt(812)
   Formula: a(n) = 114*a(n-2) - a(n-4)  (n >= 4)
   Initial terms: a(0)=28, a(1)=57, a(2)=3220, a(3)=6497 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;   /* cache[i] holds a(i) for i < cached_len */
    static idx_t  cached_len = 0;   /* number of elements stored (0..cached_len-1) */
    static idx_t  cached_cap = 0;   /* allocated capacity of cache */

    /* Initialise on first call */
    if (cache == NULL) {
        /* Ensure capacity for at least the first four terms and the requested n+1 */
        idx_t cap = (n + 1 > 4) ? (n + 1) : 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)                    /* allocation failed – return 0 as fallback */
            return (seq_t)0;
        cached_cap = cap;

        /* Initialise the first four terms */
        cache[0] = (seq_t)28;
        cache[1] = (seq_t)57;
        cache[2] = (seq_t)3220;
        cache[3] = (seq_t)6497;

        /* We already have the first four terms cached */
        cached_len = 4;
    }

    /* If we already have the requested value, return it */
    if (n < cached_len)
        return cache[n];

    /* Ensure the capacity covers up to n */
    if ((n + 1) > cached_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                    /* allocation failed – return 0 as fallback */
            return (seq_t)0;
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence */
    for (idx_t i = cached_len; i <= n; ++i) {
        /* recurrence: a(i) = 114 * a(i-2) - a(i-4) */
        seq_t term = ((seq_t)114) * cache[i - 2] - cache[i - 4];
        cache[i] = term;
    }
    cached_len = n + 1;

    return cache[n];
}
