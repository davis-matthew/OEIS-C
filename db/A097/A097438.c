/*
 * A097438  a(n) = Sum_{k|n} a(k) a(n-k) for n >= 2, a(0)=0, a(1)=1.
 * https://oeis.org/A097438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n} a(k) * a(n-k), a(0)=0, a(1)=1
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 41, u64 n <= 77, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoises computed values and evaluates each
   a(n) for n≥2 by iterating over the proper divisors of n.  The
   expression a(n)*a(0) is omitted because it is always 0 and
   avoids using the not-yet-computed a(n). */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache[i] holds the value of a(i).  The cache is grown as needed. */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated capacity */
    static idx_t   cache_last = 1;  /* the largest n whose value is already stored */
    static int     initialized = 0;

    /* Ensure that the cache can hold indices up to n. */
    if (!initialized || n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 4;
        if (new_cap < n + 1) {
            do { new_cap <<= 1; } while (new_cap < n + 1);
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* allocate failure */
        cache = tmp;
        /* Zero‑initialise any new slots */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Initialise base values once. */
    if (!initialized) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_last = 1;
        initialized = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 1) {               /* a(0) and a(1) are already set */
            cache[i] = (seq_t)i;
            continue;
        }
        seq_t sum = (seq_t)0;
        /* Iterate over proper divisors d of i (1 <= d < i) */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d != 0) continue;
            idx_t d1 = d;               /* smaller divisor */
            idx_t d2 = i / d;           /* complementary divisor */

            /* term for d1 (always < i) */
            sum += cache[d1] * cache[i - d1];

            /* term for d2, if distinct and proper */
            if (d2 != d1 && d2 < i) {
                sum += cache[d2] * cache[i - d2];
            }
        }
        cache[i] = sum;
    }
    cache_last = n;

    return cache[n];
}
