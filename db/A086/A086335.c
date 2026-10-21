/*
 * A086335  a(n) = a((n - a(n-1))*(1-mod(n, 2))+a(n-1)*(mod(n, 2))) + a((n -
 *          a(n-2))*(1-mod(n, 2)) +(n-a(n-1))*(mod(n, 2))) with a(1) = a(2) =
 *          1.
 * https://oeis.org/A086335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1. For n ≥ 3, if n is odd: a(n) = a(a(n‑1)) + a(n –
 *            a(n‑1)) if n is even: a(n) = a(n – a(n‑1)) + a(n – a(n‑2)) (This
 *            recursive definition is computed iteratively with memoisation.)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[1]..cache[cache_len] */
static size_t cache_cap = 0;     /* allocated size of cache array */
static size_t cache_len = 0;     /* highest index computed (≥2) */

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1)=a(2)=1 */
    if (n <= 2) return 1;

    /* Grow the cache array if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise minimal cache entries if not yet done */
    if (cache_len < 2) {
        cache[0] = 0;   /* unused */
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > cache_len) {
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            size_t ii = (size_t)i;
            if ((i & 1ULL) == 1ULL) {          /* odd n */
                seq_t aprev = cache[ii - 1];
                cache[ii] = cache[(size_t)aprev] + cache[ii - (size_t)aprev];
            } else {                           /* even n */
                seq_t val1 = cache[ii - 1];
                seq_t val2 = cache[ii - 2];
                cache[ii] = cache[ii - (size_t)val1] +
                            cache[ii - (size_t)val2];
            }
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
