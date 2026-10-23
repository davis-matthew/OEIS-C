/*
 * A172029  a(1) = 3; for n > 1, a(n) = smallest k such that a(n-1)^3 + k is a
 *          cube.
 * https://oeis.org/A172029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3; for n>1,
   a(n) = 3*a(n-1)^2 + 3*a(n-1) + 1
   This implements the OEIS sequence A172029.
*/

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n) {
    /* Cached sequence values.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* current allocated size (max index + 1) */
    static idx_t cached_up_to = 0; /* largest index whose value is already known */

    /* Ensure there is at least space for index 1 */
    if (cache_len < 2) {
        seq_t *tmp = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        tmp[1] = (seq_t)3;
        if (cache) free(cache);
        cache      = tmp;
        cache_len  = 2;
        cached_up_to = 1;
    }

    /* Resize cache if necessary to accommodate requested index n */
    if (n + 1 > cache_len) {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failed */
        /* Copy existing values */
        for (idx_t i = 1; i <= cached_up_to; ++i)
            new_cache[i] = cache[i];
        if (cache) free(cache);
        cache    = new_cache;
        cache_len = n + 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = prev * prev;     /* prev^2 */
        next = next * 3;              /* 3 * prev^2 */
        next = next + prev * 3;       /* + 3*prev */
        next = next + 1;              /* + 1 */
        cache[i] = next;
    }
    cached_up_to = n;

    return cache[n];
}
