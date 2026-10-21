/*
 * A066337  Decimal equivalent of A066327.
 * https://oeis.org/A066337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   not stated
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A066337
   Formula: for n > 28
     a(n) = a(n-1) - a(n-4) + a(n-5) - a(n-8) + a(n-9) - a(n-12)
            + a(n-13) + a(n-16) - a(n-17) + a(n-20) - a(n-21)
            + a(n-24) - a(n-25) + a(n-28) - a(n-29)
   Initial values are obtained from the rational generating function and
   are implicitly given by the coefficient arrays below.  This implementation
   uses a linear recurrence derived from the generating function and memoises
   all values in a dynamically‑expanded cache.  It works for any idx_t n
   (over 0) and for all supported seq_t types. */

#include <stdlib.h>
#include <stddef.h>

/* Coefficients of the numerator N(x) = Σ p[n] x^n   (degree 28) */
static const seq_t P[29] = {
    0,   2,   3,  -1,   2,   5,   2,   1,   5,   4,   4, -14,   5,
    4,   4,   4,   4,   2,   1,   5,   2,  -1,   2,   3,  -1,   0,
    0,   2,  -1
};

/* Coefficients of the denominator D(x) in the recurrence:
   a[n] = P[n] - Σ_{k=1}^{29} Q[k] * a[n-k]   */
static const int Q[30] = {
    0,   /* k = 0 unused */
   -1,   /* k = 1   : a[n-1]  coefficient  +1  → q[1] = –1 */
    0,   /* k = 2   */
    0,   /* k = 3   */
    1,   /* k = 4   : a[n-4]  coefficient –1  → q[4] = +1 */
   -1,   /* k = 5   */
    0,   /* k = 6   */
    0,   /* k = 7   */
    1,   /* k = 8   */
   -1,   /* k = 9   */
    0,   /* k =10   */
    0,   /* k =11   */
    1,   /* k =12   */
   -1,   /* k =13   */
    0,   /* k =14   */
    0,   /* k =15   */
   -1,   /* k =16   */
    1,   /* k =17   */
    0,   /* k =18   */
    0,   /* k =19   */
   -1,   /* k =20   */
    1,   /* k =21   */
    0,   /* k =22   */
    0,   /* k =23   */
   -1,   /* k =24   */
    1,   /* k =25   */
    0,   /* k =26   */
    0,   /* k =27   */
   -1,   /* k =28   */
    1    /* k =29   */
};

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of seq_t values */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static size_t cache_len  = 0;      /* number of valid entries */

    /* Ensure the cache can hold up to index n (size n+1) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 3 / 2
                                   : 64;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms from cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t value = 0;
        if (i < sizeof(P) / sizeof(P[0]))
            value = P[i];           /* coefficient from numerator */

        /* Subtract recurrence terms.  For k <= i, a[i-k] is known. */
        size_t max_k = (i < 29) ? i : 29;
        for (size_t k = 1; k <= max_k; ++k)
            value -= (seq_t)Q[k] * cache[i - k];

        cache[i] = value;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
