/*
 * A151241  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, 0, 0), (-1, 0,
 *          1), (1, 0, 0), (1, 0, 1), (1, 1, 0)}.
 * https://oeis.org/A151241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of walks in the first octant of Z³ from (0,0,0) *
 *            with n steps chosen from
 *            {(-1,0,0),(-1,0,1),(1,0,0),(1,0,1),(1,1,0)}. * The value is
 *            computed via a dynamic‑programming recurrence on the *
 *            x‑coordinate: * * f_0[0] = 1 * f_s[j] = 3·f_{s-1}[j-1] (j>0) * +
 *            2·f_{s-1}[j+1] (j+1 ≤ s-1) *...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global static data used by the function and its helpers */
static seq_t *dp_prev = NULL;   /* distribution for step s‑1 */
static seq_t *dp_curr = NULL;   /* distribution for step s   */
static size_t dp_size = 0;      /* current allocation size   */
static seq_t *cache   = NULL;   /* cached a(n) values        */
static size_t cache_cap = 0;    /* cache allocation size     */
static idx_t max_n = 0;         /* largest n computed so far */

/* Grow the DP arrays to be able to store indices up to 'need' */
static void grow_dp(idx_t need)
{
    size_t new_cap = (size_t)need + 1;
    if (new_cap <= dp_size)
        return;
    seq_t *new_prev = (seq_t *)realloc(dp_prev, new_cap * sizeof(seq_t));
    seq_t *new_curr = (seq_t *)realloc(dp_curr, new_cap * sizeof(seq_t));
    if (!new_prev || !new_curr)
        exit(1);
    /* initialise new slots to zero */
    for (size_t i = dp_size; i < new_cap; ++i) {
        new_prev[i] = 0;
        new_curr[i] = 0;
    }
    dp_prev = new_prev;
    dp_curr = new_curr;
    dp_size = new_cap;
}

/* Grow the cache array to be able to store indices up to 'need' */
static void grow_cache(idx_t need)
{
    size_t new_cap = (size_t)need + 1;
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    /* initialise new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    /* Initialise data structures on first call */
    if (dp_prev == NULL) {
        dp_size = 1;
        dp_prev = (seq_t *)malloc(dp_size * sizeof(seq_t));
        dp_curr = (seq_t *)malloc(dp_size * sizeof(seq_t));
        if (!dp_prev || !dp_curr)
            exit(1);
        dp_prev[0] = 1;   /* f_0[0] = 1 */
        dp_curr[0] = 0;
        /* initialise cache */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = 1;     /* a(0) = 1 */
        max_n = 0;
    }

    /* Compute up to n if not yet done */
    if (n > max_n) {
        for (idx_t step = max_n + 1; step <= n; ++step) {
            grow_dp(step);               /* ensure arrays large enough */

            seq_t sum = 0;
            /* compute dp_curr for this step */
            for (idx_t j = 0; j <= step; ++j) {
                seq_t val = 0;
                if (j > 0)                 /* from state j-1 via +1 steps */
                    val += (seq_t)3 * dp_prev[j - 1];
                if (j + 1 <= step - 1)     /* from state j+1 via -1 steps */
                    val += (seq_t)2 * dp_prev[j + 1];
                dp_curr[j] = val;
                sum += val;
            }

            grow_cache(step);            /* ensure cache large enough */
            cache[step] = sum;

            /* swap dp_prev and dp_curr for next iteration */
            seq_t *tmp = dp_prev;
            dp_prev = dp_curr;
            dp_curr = tmp;
        }
        max_n = n;
    }

    return cache[n];
}
