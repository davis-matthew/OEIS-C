/*
 * A160131  Numerator of Hermite(n, 8/27).
 * https://oeis.org/A160131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 8/27) Recurrence: a(0)=1, a(1)=16,
 *            a(n+1) = 16*a(n) - 1458*n*a(n-1) for n >= 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms      */
    static size_t cap   = 0;      /* current capacity of cache    */
    static size_t last  = 0;      /* largest index already filled */

    /* Ensure the cache is large enough for index n */
    size_t idx = (size_t)n;
    if (idx + 1 > cap) {
        size_t newcap = (cap ? cap * 2 : 16);
        if (newcap <= idx) newcap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed – return 0 */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first two terms if not already done */
    if (cap >= 2 && last < 1) {
        cache[0] = (seq_t)1;             /* a(0) */
        cache[1] = (seq_t)16;            /* a(1) */
        last    = 1;
    }

    /* Compute any terms that are needed up to n */
    if (idx > last) {
        for (size_t i = last + 1; i <= idx; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            /* recurrence: a(i) = 16*a(i-1) - 1458*(i-1)*a(i-2) */
            cache[i] = 16 * prev1 - 1458 * ((seq_t)(i - 1)) * prev2;
        }
        last = idx;
    }

    return cache[idx];
}
