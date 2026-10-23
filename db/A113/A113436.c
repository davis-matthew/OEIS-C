/*
 * A113436  First row of A113435.
 * https://oeis.org/A113436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 7*a(n-1) - 15*a(n-2) + 11*a(n-3) - a(n-4),
 *            a(0)=1, a(1)=2, a(2)=7, a(3)=26
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t cache_len = 0;   /* next free index in cache */

/* Pre‑initialised first four terms */
static const seq_t init_vals[4] = { (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)26 };

seq_t a(idx_t n)
{
    /* Ensure cache has the initial four terms */
    if (cache_len == 0)
    {
        size_t init_sz = 4;
        cache = (seq_t *)malloc(init_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        cache_len = 4;
    }

    if (n < cache_len)
        return cache[n];

    /* Extend cache up to n (inclusive) */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failed */
    cache = tmp;

    /* Compute new values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        cache[i] = (seq_t)7  * cache[i-1]
                 - (seq_t)15 * cache[i-2]
                 + (seq_t)11 * cache[i-3]
                 -        cache[i-4];
    }
    cache_len = new_len;
    return cache[n];
}
