/*
 * A034787  a(n) = n-th sextic factorial number divided by 5.
 * https://oeis.org/A034787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∏_{j=1..n} (6*j-1))/5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = (6*n-1) * a(n-1) for n ≥ 2                     */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 1.  For safety return 0 for n = 0. */
    if (n == 0)
        return 0;

    /* Initialise the cache on first call.  Index 0 is unused. */
    if (cache == NULL)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)   /* allocation failed – return 0 as a fallback */
            return 0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cache_max = 1;
    }

    /* Grow the cache if a larger index is requested. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp)               /* reallocation failed – return 0 */
            return 0;
        cache = newp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence a(n) = (6*n-1)*a(n-1). */
    if (n > cache_max)
    {
        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            seq_t factor = (seq_t)((idx_t)6 * i - (idx_t)1);   /* 6*i-1 */
            cache[i] = cache[i - 1] * factor;
        }
        cache_max = n;
    }

    return cache[n];
}
