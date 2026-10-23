/*
 * A186607  Number of (n+4)X11 binary arrays with every 5X5 subblock commuting
 *          with each horizontal and vertical neighbor 5X5 subblock.
 * https://oeis.org/A186607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4)+a(n-5)-a(n-6)-a(n-8)
 *            +a(n-9)+a(n-10)-a(n-13) for n>21 Initial values for 1 ≤ n ≤ 21
 *            are given in the OEIS entry
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for the unused index 0. */
    if (n == 0) return (seq_t)0;

    /* Known values a(1) … a(21). */
    static const seq_t init[22] = {
        (seq_t)0,          /* dummy a(0) */
        (seq_t)707955,
        (seq_t)38625,
        (seq_t)61336,
        (seq_t)98376,
        (seq_t)132040,
        (seq_t)176696,
        (seq_t)191232,
        (seq_t)193000,
        (seq_t)195032,
        (seq_t)198284,
        (seq_t)206372,
        (seq_t)229900,
        (seq_t)274644,
        (seq_t)326864,
        (seq_t)405664,
        (seq_t)549188,
        (seq_t)715968,
        (seq_t)905732,
        (seq_t)1152093,
        (seq_t)1428286,
        (seq_t)1777291
    };

    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;   /* cache[0 .. cached] at least */
    static idx_t cached = 0;        /* maximum n stored */
    static idx_t cap    = 0;        /* allocated size of cache */

    /* Initial allocation (size at least 22). */
    if (cache == NULL) {
        cap = 22;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        for (idx_t i = 1; i <= 21; ++i)
            cache[i] = init[i];
        cached = 21;
    }

    /* If we already have the requested value, return it. */
    if (n <= cached)
        return cache[n];

    /* Grow the cache if necessary. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms from cached+1 up to n using the recurrence. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* For i <= 21, values already set. */
        if (i <= 21) {
            /* Already initialized from init array. */
            continue;
        }
        /* Apply recurrence: a(i) = a(i-1)+a(i-3)-a(i-4)+a(i-5)-a(i-6)
                               -a(i-8)+a(i-9)+a(i-10)-a(i-13) */
        seq_t val = cache[i - 1];
        val += cache[i - 3];
        val -= cache[i - 4];
        val += cache[i - 5];
        val -= cache[i - 6];
        val -= cache[i - 8];
        val += cache[i - 9];
        val += cache[i - 10];
        val -= cache[i - 13];
        cache[i] = val;
    }
    cached = n;
    return cache[n];
}
