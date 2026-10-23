/*
 * A169023  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤23: a(n)=8·∑_{k=1}^n a(n‑k)+2; for n≥24:
 *            a(n)=8·∑_{k=1}^{23} a(n‑k) – 36·a(n‑24) + (n==24 ? 1 : 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoized values a[0..k]   */
static idx_t  cache_len = 0;      /* number of computed entries   */
static idx_t  cache_cap = 0;      /* allocated capacity            */

/* Ensure that cache contains entries up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* Grow capacity to at least n+1, doubling to reduce reallocs. */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < n + 1)
            new_cap <<= 1;                  /* multiply by 2 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                         /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new entries, starting from the first missing index. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum the previous up to 23 values (if they exist). */
        for (idx_t k = 1; k <= 23 && i >= k; ++k)
            sum += cache[i - k];

        seq_t pn = 0;
        if (i == 0)
            pn = 1;
        else if (i <= 23)
            pn = 2;
        else if (i == 24)
            pn = 1;

        seq_t val = pn + sum * (seq_t)8;
        if (i >= 24)
            val -= cache[i - 24] * (seq_t)36;

        cache[i] = val;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
