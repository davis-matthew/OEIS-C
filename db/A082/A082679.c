/*
 * A082679  Number of LEGO towers, one piece per floor, where every floor is
 *          perpendicular to the one below it (so we have a kind of
 *          3-dimensional zigzag pattern).
 * https://oeis.org/A082679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 3*a(n-3) - a(n-4) (n >= 5) a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=20
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;  /* highest index computed so far */

    /* initialise cache with first five terms */
    if (cache == NULL) {
        cache = (seq_t *)calloc(5, sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)20;
        max_computed = 4;
    }

    if (n <= max_computed)
        return cache[n];

    /* extend cache to required index */
    size_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* compute remaining terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a[i] = 5*a[i-1] - 6*a[i-2] + 3*a[i-3] - a[i-4] */
        seq_t val = cache[i - 1] * (seq_t)5
                     - cache[i - 2] * (seq_t)6
                     + cache[i - 3] * (seq_t)3
                     - cache[i - 4];
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
