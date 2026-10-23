/*
 * A198964  a(n) = (7*9^n - 1)/2.
 * https://oeis.org/A198964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*9^n - 1)/2 (equivalently a(n) = 9*a(n-1)+4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache: a[0 .. last] */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;   /* allocated size of cache   */
    static idx_t  last    = 0;   /* largest n computed so far */

    /* First call: allocate minimal cache and initialise a(0) */
    if (cache == NULL) {
        cap   = (n > 0) ? n + 1 : 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = 3;                         /* a(0) = 3 */
        last = 0;
    }

    /* Grow the cache if necessary */
    if (cache != NULL && (n + 1 > cap)) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* realloc failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * 9 + 4;   /* recurrence a(i) = 9 * a(i-1) + 4 */
        }
        last = n;
    }

    return cache[n];
}
