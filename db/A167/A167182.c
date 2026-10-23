/*
 * A167182  a(0) = 1, a(1) = 2; for n >= 2, a(n) = 2^A042950(n-2).
 * https://oeis.org/A167182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2. For n≥2, a(n)=2^A042950(n-2). Equivalently,
 *            a(2)=4, a(3)=8 and for n≥4 a(n)=a(n‑1)^2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;        /* pointer to sequence array   */
static size_t cache_len = 0;       /* number of values stored     */
static size_t cache_cap = 0;       /* allocated capacity in slots */

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* if already computed, return it */
    if (idx < cache_len)
        return cache[idx];

    /* enlarge the array if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        if (new_cap < 2 * cache_cap)   /* grow by a factor of 2 */
            new_cap = 2 * cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);             /* memory exhaustion – abort */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)2;
        else if (i == 2)     cache[i] = (seq_t)4;
        else if (i == 3)     cache[i] = (seq_t)8;
        else                 cache[i] = cache[i-1] * cache[i-1]; /* square */
    }
    cache_len = idx + 1;

    return cache[idx];
}
