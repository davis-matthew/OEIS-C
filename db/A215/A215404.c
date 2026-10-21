/*
 * A215404  a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3), with a(0)=0, a(1)=0 and
 *          a(2)=1.
 * https://oeis.org/A215404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Constants cast to seq_t */
    static const seq_t ZERO  = (seq_t)0;
    static const seq_t ONE   = (seq_t)1;
    static const seq_t FOUR  = (seq_t)4;
    static const seq_t THREE = (seq_t)3;

    /* Memoisation cache */
    static seq_t *cache      = NULL;   /* array of terms */
    static idx_t   cache_cap = 0;      /* allocated size */
    static idx_t   cache_len = 0;      /* number of initialised terms */

    /* Initialise cache on first call */
    if (cache == NULL)
    {
        /* Ensure capacity for up to n */
        idx_t cap = n + 1;
        if (cap < 3) cap = 3;           /* need at least a(0)..a(2) */
        cache_cap = cap;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return ZERO;    /* memory error: return 0 */

        cache[0] = ZERO;
        cache[1] = ZERO;
        cache[2] = ONE;
        cache_len = 3;                     /* we have a(0)..a(2) ready */
    }
    /* Grow cache if needed */
    else if (cache_cap < n + 1)
    {
        idx_t new_cap = n + 1;
        if (new_cap < 64) new_cap = 64;    /* small optimisation */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return ZERO;     /* memory error: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    if (cache_len <= n)
    {
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t val = FOUR * cache[i - 1];
            val -= THREE * cache[i - 2];
            val -= cache[i - 3];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
