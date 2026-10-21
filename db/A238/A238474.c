/*
 * A238474  a(n) = (-1)^n*(n+3)!/(2*(n+1)) for n >= 0.
 * https://oeis.org/A238474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*(n+3)!/(2*(n+1))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values   */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache for n = 0 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed – unlikely */
        cache[0] = (seq_t)3;                  /* a(0) = 3 */
        cache_len = 1;
    }

    /* extend cache up to index n (if needed) */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed – unlikely */
        cache = tmp;

        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence: a[i] = -a[i-1] * (i+3) * i / (i+1)  */
            seq_t prev = cache[i - 1];

            idx_t denom = (idx_t)(i + 1);      /* (i+1)          */
            idx_t num   = (idx_t)i;           /* i              */
            idx_t prod1 = (idx_t)(i + 3);     /* (i+3)          */

            /* cancel factors of 'denom' with 'prev', 'num', and 'prod1' */
            seq_t g = gcd(prev < 0 ? -prev : prev, denom);
            prev /= g;
            denom /= (idx_t)g;

            g = gcd(num, denom);
            num   /= (idx_t)g;
            denom /= (idx_t)g;

            g = gcd(prod1, denom);
            prod1 /= (idx_t)g;
            denom /= (idx_t)g;

            /* After cancellations denom should be 1 */
            (void)denom;    /* unused, kept for clarity */

            /* compute result, keeping the sign */
            seq_t result = -prev;
            result *= (seq_t)prod1;
            result *= (seq_t)num;

            cache[i] = result;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
