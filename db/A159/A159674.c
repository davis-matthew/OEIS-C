/*
 * A159674  Expansion of (1 - x)/(1 - 32*x + x^2).
 * https://oeis.org/A159674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=31, a(n)=32*a(n-1)-a(n-2) for n>=2 (OEIS sequence
 *            A159674: expansion of (1 - x)/(1 - 32*x + x^2))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are defined elsewhere by the build system. */

/* Static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of terms stored */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Grow cache if needed */
    size_t target = (size_t)(n + 1);
    if (target > cache_len)
    {
        /* Reallocate cache to hold at least target terms */
        seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache)
        {
            /* Allocation failed – return 0 (caller must not rely on this) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Initialize base cases if they haven't been set yet */
        size_t start = cache_len;  /* index from which to compute new terms */

        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            if (target > 1)
                cache[1] = (seq_t)31;
            start = 2;
        } else if (cache_len == 1) {
            if (target > 1)
                cache[1] = (seq_t)31;
            start = 2;
        }

        /* Compute remaining terms using the recurrence */
        for (size_t i = start; i < target; ++i) {
            cache[i] = (seq_t)32 * cache[i - 1] - cache[i - 2];
        }

        cache_len = target;
    }

    return cache[(size_t)n];
}
