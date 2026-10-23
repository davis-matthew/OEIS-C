/*
 * A143075  Polynomial expansion sequence: p(x)=1/(1 - 4x + 5x^2 - 6x^4 + 6x^5
 *          - x^6 - 2x^7 + x^8).
 * https://oeis.org/A143075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) -5*a(n-2) +6*a(n-4) -6*a(n-5) + a(n-6) +2*a(n-7)
 *            - a(n-8) for n ≥ 1, with a(0) = 1. This follows from the
 *            expansion of 1/(1 - 4x + 5x^2 - 6x^4 + 6x^5 - x^6 - 2x^7 + x^8)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of the sequence   */
static idx_t  cache_cap = 0;         /* allocated size of cache        */
static idx_t  cache_max = 0;         /* largest index that has been computed */

static void
ensure_cache(idx_t n)
{
    /* allocate or grow the cache if necessary                            */
    if (cache == NULL) {
        cache_cap = (n + 1) < 8 ? 8 : (n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;            /* a(0) = 1                      */
        cache_max = 0;
    } else if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n                                       */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = (seq_t)4 * cache[i - 1];
            if (i >= 2) val -= (seq_t)5 * cache[i - 2];
            if (i >= 4) val += (seq_t)6 * cache[i - 4];
            if (i >= 5) val -= (seq_t)6 * cache[i - 5];
            if (i >= 6) val += cache[i - 6];
            if (i >= 7) val += (seq_t)2 * cache[i - 7];
            if (i >= 8) val -= cache[i - 8];
            cache[i] = val;
        }
        cache_max = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
