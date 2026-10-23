/*
 * A041189  Denominators of continued fraction convergents to sqrt(105).
 * https://oeis.org/A041189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 82*a(n-2) - a(n-4); a(0)=1, a(1)=4, a(2)=81, a(3)=328
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = (idx_t)-1;   /* indicates uninitialized */

    /* initial allocation and base values */
    if (cache == NULL) {
        /* we always allocate at least 4 elements for the initial terms */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)81;
        cache[3] = (seq_t)328;
        max_computed = 3;
    }

    /* extend cache if the requested n is beyond what we have computed */
    if (n > max_computed) {
        /* reallocate to hold up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) /* allocation failed; return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i) = 82*a(i-2) - a(i-4) */
            cache[i] = (seq_t)82 * cache[i - 2] - cache[i - 4];
        }
        max_computed = n;
    }

    return cache[n];
}
