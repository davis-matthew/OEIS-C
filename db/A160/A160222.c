/*
 * A160222  Numerator of Hermite(n, 25/28).
 * https://oeis.org/A160222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 14^n * Hermite(n, 25/28) Recurrence: a(0) = 1, a(1) = 25,
 *            a(n) = 25*a(n-1) - 392*(n-1)*a(n-2) (n ≥ 2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t supports standard arithmetic, indexing and construction
   from integer literals.  The cache is stored in static storage so that
   calls may be made in any order. */

#include <stdlib.h>   /* for size_t, realloc */

seq_t a(idx_t n)
{
    /* Cache for previously computed terms.  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms currently stored */

    /* If the term is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Resize the cache to hold terms up to index n. */
    size_t needed = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache)
        return 0;                     /* allocation failed – unlikely in tests */

    cache = new_cache;

    /* Compute any missing terms. */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)25;
        } else {
            /* a(k) = 25*a(k-1) - 392*(k-1)*a(k-2) */
            seq_t term1 = (seq_t)25 * cache[k - 1];
            seq_t term2 = (seq_t)392 * (seq_t)(k - 1) * cache[k - 2];
            cache[k] = term1 - term2;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
