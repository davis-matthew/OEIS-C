/*
 * A169101  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for 1≤n≤24 a(n)=2+38·∑_{k=0}^{n-1}a(k), *
 *            a(25)=1+38·∑_{k=1}^{24}a(k)−741·a(0), * for n≥26
 *            a(n)=38·∑_{k=n-24}^{n-1}a(k)−741·a(n-25) * (derived from the
 *            given generating function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = (idx_t)-1;          /* highest index computed (sentinel) */
static seq_t cum_sum = 0;                    /* Σ_{k=0}^{i} a(k) after a(i) written */
static seq_t sum_last24 = 0;                 /* Σ_{k=i-24}^{i-1} a(k) when i≥25 */

/* ensure the cache array can hold indices up to need-1 */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* one‑time initialisation */
    if (cache_max == (idx_t)-1) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_max = 0;
        cum_sum = (seq_t)1;
    }

    /* cached result? */
    if (n <= cache_max)
        return cache[n];

    /* extend cache up to n */
    ensure_capacity((size_t)n + 1);

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val;
        if (k < 25) {
            /* a(k) = 2 + 38 * Σ_{i=0}^{k-1} a(i)   */
            val = (seq_t)2 + (seq_t)38 * cum_sum;
            cum_sum += val;
            cache[k] = val;
        } else if (k == 25) {
            /* sum of a(1)..a(24) */
            sum_last24 = cum_sum - cache[0];
            val = (seq_t)1 + (seq_t)38 * sum_last24 - (seq_t)741 * cache[0];
            cache[k] = val;
            cum_sum += val;
            sum_last24 = sum_last24 + val - cache[1];   /* window for next n */
        } else {            /* k > 25  */
            val = (seq_t)38 * sum_last24 - (seq_t)741 * cache[k-25];
            cache[k] = val;
            cum_sum += val;                                /* not needed later */
            sum_last24 = sum_last24 + val - cache[k-24];     /* slide window */
        }
    }

    cache_max = n;
    return cache[n];
}
