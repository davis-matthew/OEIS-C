/*
 * A186133  Number of (n+1) X 2 0..2 arrays with no 2 X 2 subblock trace equal
 *          to any horizontal or vertical neighbor 2 X 2 subblock trace.
 * https://oeis.org/A186133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 81, a(2) = 558, a(3) = 3888, a(n) = 3*a(n-1) + 24*a(n-2)
 *            + 24*a(n-3) (for n ≥ 4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A186133 – Number of (n+1) X 2 0..2 arrays with no 2 X 2 subblock trace equal to any horizontal or vertical neighbor 2 X 2 subblock trace. */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* cache: 0‑based array storing a(1) at index 0, a(2) at 1, … */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated capacity (number of seq_t elements) */
    static idx_t  cache_max = 0;   /* maximum n computed so far */

    if (n == 0) return 0;          /* offset is 1 – undefined behaviour below, but return 0 */

    /* Allocate or grow the cache if necessary. */
    if (cache == NULL) {
        cache_cap = n + 1;     /* ensure capacity for at least n terms */
        if (cache_cap < 4) cache_cap = 4;          /* handle small n            */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (n + 1 > cache_cap) {
        idx_t newcap = cache_cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* If already computed, just return. */
    if (n <= cache_max) return cache[n - 1];

    /* Compute terms from cache_max+1 up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = 81;
        else if (i == 2)     val = 558;
        else if (i == 3)     val = 3888;
        else {
            /* recurrence: a(i) = 3*a(i-1) + 24*a(i-2) + 24*a(i-3) */
            seq_t a1 = cache[i - 2];   /* a(i-1) */
            seq_t a2 = cache[i - 3];   /* a(i-2) */
            seq_t a3 = cache[i - 4];   /* a(i-3) */
            val = 3 * a1 + 24 * a2 + 24 * a3;
        }
        cache[i - 1] = val;
    }

    cache_max = n;
    return cache[n - 1];
}
