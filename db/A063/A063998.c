/*
 * A063998  Nonsquare, composite numbers k such that Sum_{d|k, d < k/d} s_d =
 *          k+1, where s_d is either d+1/(k/d) or (1/d)+(k/d).
 * https://oeis.org/A063998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Nonsquare composite numbers k such that Sum_{d|k, d < k/d} s_d =
 *            k+1, where for each divisor pair (d, k/d) s_d is either d +
 *            1/(k/d) or (1/d) + (k/d)
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063998 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

/* recursive subset‑sum solver with pruning using suffix sums */
static bool dfs_subset(const uint64_t *w, size_t m, size_t idx,
                       uint64_t cur, uint64_t target,
                       const uint64_t *suff)
{
    if (cur == target)
        return true;
    if (idx == m)
        return false;
    if (cur + suff[idx] < target)
        return false;
    if (cur + w[idx] <= target)
        if (dfs_subset(w, m, idx + 1, cur + w[idx], target, suff))
            return true;
    return dfs_subset(w, m, idx + 1, cur, target, suff);
}

/* static cache for computed terms */
static seq_t *cache = NULL;      /* array of terms */
static idx_t  cache_cap = 0;     /* capacity of cache array */
static idx_t  cache_len = 0;     /* number of stored terms */

/* current number to test for next sequence term */
static uint64_t cur_k = 2;

/* enlarge cache to accommodate at least 'need' elements */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* integer square root using double for approximation, then adjust */
static inline uint64_t isqrt64(uint64_t n)
{
    long double r = sqrt((long double)n);
    uint64_t x = (uint64_t)r;
    while ((unsigned __int128)(x + 1) * (x + 1) <= n)
        ++x;
    while ((unsigned __int128)x * x > n)
        --x;
    return x;
}

/* test whether k belongs to the sequence */
static bool qualifies(uint64_t k)
{
    if (k < 6)
        return false;               /* smallest composite nonsquare is 6 */

    uint64_t r = isqrt64(k);
    if (r * r == k)                 /* perfect square */
        return false;

    /* arrays for divisor pairs */
    uint64_t w[128];
    size_t   m = 0;
    uint64_t sum_d = 0;
    bool     composite = false;

    for (uint64_t d = 1; d <= r; ++d) {
        if (k % d == 0) {
            uint64_t other = k / d;
            if (d < other) {
                if (d > 1)
                    composite = true; /* found a proper divisor */
                sum_d += d;
                w[m++] = other - d;
            }
        }
    }
    if (!composite)                 /* prime */
        return false;
    if (m == 0)
        return false; /* should not happen for composite nonsquare */

    uint64_t target = k - sum_d;
    uint64_t sum_w = 0;
    for (size_t i = 0; i < m; ++i)
        sum_w += w[i];
    if (target == 0 || target > sum_w)
        return false;

    /* build suffix sums for pruning */
    uint64_t suff[128];
    suff[m] = 0;
    for (int i = (int)m - 1; i >= 0; --i)
        suff[i] = suff[i + 1] + w[i];

    return dfs_subset(w, m, 0, 0, target, suff);
}

/* add a new term to the cache */
static void add_term(uint64_t val)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = (seq_t)val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined, but prevent crash */

    while (cache_len < n) {
        while (!qualifies(cur_k))
            ++cur_k;
        add_term(cur_k);
        ++cur_k;
    }
    return cache[n - 1];
}
