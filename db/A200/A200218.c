/*
 * A200218  The differences x^3 - y^2 of Danilov's subsequence of good Hall's
 *          examples A078933.
 * https://oeis.org/A200218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -1860497*a(n‑1) + 1860497*a(n‑2) + a(n‑3) initial values:
 *            a(1) = -297 a(2) = 548147655 a(3) = -1019827620252441
 * Range:     i8 none, i16 n <= 1, i32 n <= 2, i64 n <= 3, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour for n<1 is irrelevant. */
    if (n < 1)
        return 0;

    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;    /* Array of terms, 0‑based */
    static size_t cap   = 0;       /* Allocated capacity (in terms) */
    static idx_t  max_n = 0;       /* Largest n that has been computed   */

    /* Ensure the cache is large enough for the requested index. */
    if (n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();        /* Allocation failed */
        cache = tmp;
        cap   = n;
    }

    /* Compute any missing values up to n (inclusive). */
    for (idx_t i = max_n; i < n; ++i) {
        if (i == 0) {            /* a(1) */
            cache[0] = (seq_t)(-297);
        } else if (i == 1) {     /* a(2) */
            cache[1] = (seq_t)548147655;
        } else if (i == 2) {     /* a(3) */
            cache[2] = (seq_t)-1019827620252441LL;
        } else {                 /* recurrence for n >= 4 */
            seq_t a1 = cache[i - 1];   /* a(i)   */
            seq_t a2 = cache[i - 2];   /* a(i-1) */
            seq_t a3 = cache[i - 3];   /* a(i-2) */
            cache[i] = -( (seq_t)1860497 * a1 )
                       + ( (seq_t)1860497 * a2 )
                       + a3;
        }
    }

    max_n = n;
    return cache[n - 1];
}
