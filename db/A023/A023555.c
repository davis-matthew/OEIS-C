/*
 * A023555  Self-convolution of A023531.
 * https://oeis.org/A023555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n-1} b(k)·b(n+1–k) , where b(m) = 1 if m =
 *            t·(t+3)/2 for some integer t ≥ 1, otherwise 0. Thus a(n) counts
 *            ordered pairs of indices of the self‑convolution of A023531
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache structure: values up to cache_limit are stored in cache[0..limit] */
static seq_t *cache       = NULL;   /* values of a(k) for k ≤ cache_limit */
static size_t cache_size  = 0;      /* actual allocated size of cache array   */
static idx_t cache_limit = 0;      /* largest n for which cache is valid     */

/* Precompute a(k) for all 1 ≤ k ≤ limit, storing them in cache. */
static void precompute(idx_t limit)
{
    if (limit <= cache_limit)
        return;                               /* nothing to do */

    /* Build a list of indices where b(i)=1 (i = t*(t+3)/2). */
    idx_t *pos   = NULL;
    size_t pos_cap = 0, pos_cnt = 0;
    for (idx_t t = 1; ; ++t)
    {
        idx_t v = t * (t + 3) / 2;
        if (v > limit)
            break;
        if (pos_cnt >= pos_cap)
        {
            size_t new_cap = pos_cap ? pos_cap * 2 : 16;
            idx_t *tmp = (idx_t *)realloc(pos, new_cap * sizeof(idx_t));
            if (!tmp)
                abort();
            pos = tmp;
            pos_cap = new_cap;
        }
        pos[pos_cnt++] = v;
    }

    /* Allocate new cache array, zeroed. */
    size_t new_cap = (size_t)limit + 1;          /* indices 0..limit */
    seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_cache)
        abort();

    /* Compute convolution: count ordered pairs (i,j) with i,j∈pos and i+j-1 ≤ limit. */
    for (size_t ii = 0; ii < pos_cnt; ++ii)
        for (size_t jj = 0; jj < pos_cnt; ++jj)
        {
            idx_t idx = pos[ii] + pos[jj] - 1;
            if (idx <= limit)
                new_cache[idx] += (seq_t)1;
        }

    /* Replace old cache. */
    free(cache);
    free(pos);
    cache       = new_cache;
    cache_size  = new_cap;
    cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    precompute(n);
    return cache[n];
}
