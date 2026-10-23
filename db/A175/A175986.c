/*
 * A175986  a(1) = 1, for n >= 2: a(n) = (a(n-1)-1)^(n-1) - 1.
 * https://oeis.org/A175986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n >= 2: a(n) = (a(n-1)-1)^(n-1) - 1
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 5, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"   /* seq_t seq_pow(seq_t base, idx_t exp); */

/* Ensure that *arr has at least n elements (capacity >= n). */
static void ensure_cache(idx_t n, seq_t **arr, size_t *cap)
{
    if (*cap < n) {
        size_t newcap = (*cap) ? (*cap) * 2 : 1;
        while (newcap < n) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;      /* array of terms          */
    static idx_t cache_cnt = 0;      /* number of terms stored  */
    static size_t cache_cap = 0;     /* allocated capacity      */

    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    if (cache_cnt < n) {
        /* Grow cache if needed. */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t i = cache_cnt + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 2];     /* a(i-1) */
                seq_t base = prev - (seq_t)1; /* a(i-1)-1 */
                seq_t pwr  = seq_pow(base, i - 1); /* (a(i-1)-1)^(i-1) */
                cache[i - 1] = pwr - (seq_t)1;     /* a(i) */
            }
        }
        cache_cnt = n;
    }
    return cache[n - 1];
}
