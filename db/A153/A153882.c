/*
 * A153882  a(n) = ((6 + sqrt(5))^n - (6 - sqrt(5))^n)/(2*sqrt(5)).
 * https://oeis.org/A153882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6 + sqrt(5))^n - (6 - sqrt(5))^n)/(2*sqrt(5))
 *            Recurrence: a(0)=0, a(1)=1, a(n)=12*a(n-1)-31*a(n-2); offset 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 850 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for 0 <= i <= cache_len-1.
       cache[0] is 0, cache[1] is 1.  All values beyond a(1) are computed on demand. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache == NULL) {
        /* Allocate space for indices 0 and 1 */
        cache = mem_new(seq_t, 2);
        if (cache == NULL)
            abort();
        cache[1] = (seq_t)1;           /* a(1) = 1 */
        cache_len = 2;                 /* indices 0 and 1 are valid */
    }

    size_t idx = (size_t)n;            /* n >= 1 */

    /* Grow the cache if the requested index exceeds current size. */
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (tmp == NULL)
            abort();
        cache = tmp;
        /* cache_len remains unchanged until new terms are filled. */
    }

    /* Compute any missing terms up to idx. */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t t1 = mul_small(cache[i - 1], 12);
        seq_t t2 = mul_small(cache[i - 2], 31);
        cache[i] = t1 - t2;
    }

    /* Update cache length to include new terms. */
    cache_len = idx + 1;

    return cache[idx];
}
