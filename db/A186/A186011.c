/*
 * A186011  1/8192 the number of (n+2) X 3 0..3 arrays with no 3 X 3 subblock
 *          trace equal to any horizontal or vertical neighbor 3 X 3 subblock
 *          trace.
 * https://oeis.org/A186011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186011
 * Empirical recurrence: a(n) = 32*a(n-1) + 1101*a(n-2) + 9060*a(n-3)
 *                        + 25452*a(n-4) + 19440*a(n-5)
 * Initial conditions: a(1)=32, a(2)=1758, a(3)=96924, a(4)=5341590, a(5)=294396948
 * The function computes a(n) for any n≥1, memoizing previous values.
 * Recurrence holds for all n≥6.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* Memoized terms, 1‑indexed (cache[0] unused) */
static idx_t cache_last = 0;     /* Highest index for which cache is defined */
static size_t cache_cap = 0;     /* Allocated capacity in elements (including cache[0]) */

/* Ensure cache is computed up to index n (n≥1) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_last)
        return;

    /* Grow capacity if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 16;
        if (new_cap < cache_cap + 16)
            new_cap = cache_cap + 16;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) { /* Allocation failed – abort silently */
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases if not already set */
    if (cache_last < 1) {
        cache[1] = (seq_t)32;
        cache_last = 1;
    }
    if (cache_last < 2) {
        cache[2] = (seq_t)1758;
        cache_last = 2;
    }
    if (cache_last < 3) {
        cache[3] = (seq_t)96924;
        cache_last = 3;
    }
    if (cache_last < 4) {
        cache[4] = (seq_t)5341590;
        cache_last = 4;
    }
    if (cache_last < 5) {
        cache[5] = (seq_t)294396948;
        cache_last = 5;
    }

    /* Compute remaining terms using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];
        cache[i] = (seq_t)32  * t1
                 + (seq_t)1101 * t2
                 + (seq_t)9060 * t3
                 + (seq_t)25452 * t4
                 + (seq_t)19440 * t5;
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
