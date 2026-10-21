/*
 * A204240  Determinant of the n-th principal submatrix of A204158.
 * https://oeis.org/A204240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 25, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -10*a(n-1)-25*a(n-2);
   Initial values: a(1) = 1, a(2) = -14 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), ... */
    static idx_t cached_n = 0;       /* number of terms currently stored */

    if (n == 0) return 0;            /* offset is 1, safety */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-14;

    /* initialize cache if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed – undefined for sequences */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-14;
        cached_n = 2;
    }

    /* extend cache up to n */
    if (n > cached_n) {
        size_t newsize = (size_t)n * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, newsize);
        if (!tmp) return 0;          /* allocation failed – undefined for sequences */
        cache = tmp;
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            seq_t val = (seq_t)(-10) * cache[k - 2] + (seq_t)(-25) * cache[k - 3];
            cache[k - 1] = val;
        }
        cached_n = n;
    }

    return cache[n - 1];
}
