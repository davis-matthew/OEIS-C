/*
 * A170536  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 45: a(n) = 45·∑_{i=0}^{n-1} a(i) + 2 For n
 *            = 46: a(46) = 45·∑_{i=1}^{45} a(i) – 1035·a(0) + 1 For n ≥ 47:
 *            a(n) = 45·∑_{i=n-45}^{n-1} a(i) – 1035·a(n-46)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Because the generating function is (t^46 + 2t^45 + … + 2t + 1)
/*  divided by (1 – 45t – … – 45t^45 + 1035t^46). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* memoisation cache */
static seq_t *cache      = NULL; /* array of computed a(n) */
static size_t cache_size = 0;    /* allocated size */
static size_t cached_len = 0;    /* number of terms already computed */

static seq_t sum_all = 0;        /* Σ a(0)…a(k)  for k ≤ 45                */
static seq_t sum_win = 0;        /* Σ a(k-45)…a(k-1) for current k (k≥46) */
static bool   win_ready = false;/* is sum_win initialised?                */

/* ensure capacity for at least (n+1) terms */
static void ensure_cache(idx_t n)
{
    if (cache_size <= (size_t)n) {
        size_t new_cap = n + 1;
        if (new_cap < 64)   new_cap = 64;          /* give some headroom   */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* out of memory        */
        cache      = tmp;
        cache_size = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* generate missing terms up to n */
    while (cached_len <= n) {
        if (cached_len == 0) {
            /* a(0) = 1 */
            cache[0] = (seq_t)1;
            sum_all  = cache[0];
            cached_len = 1;
            continue;
        }

        if (cached_len <= 45) {
            /* 1 ≤ k ≤ 45 : a(k) = 45·Σ_{i=0}^{k-1} a(i) + 2 */
            seq_t val = (seq_t)45 * sum_all + (seq_t)2;
            cache[cached_len] = val;
            sum_all += val;
            cached_len++;
            continue;
        }

        if (cached_len == 46) {
            /* a(46) = 45·Σ_{i=1}^{45} a(i) – 1035·a(0) + 1 */
            seq_t sum1_45 = sum_all - cache[0];           /* Σ a[1]…a[45]   */
            seq_t val = (seq_t)45 * sum1_45 - (seq_t)1035 * cache[0] + (seq_t)1;
            cache[cached_len] = val;
            cached_len++;

            /* initialise sliding window: Σ a[2]…a[46] */
            sum_win = (seq_t)0;
            for (idx_t j = 2; j <= 46; ++j)
                sum_win += cache[j];
            win_ready = true;
            continue;
        }

        /* cached_len ≥ 47 : recurrence without p_n term */
        if (!win_ready) {
            /* In strange cases where cached_len jumps to >46
               without initialising sum_win (unlikely, but safe). */
            sum_win = (seq_t)0;
            for (idx_t j = cached_len - 45; j < cached_len; ++j)
                sum_win += cache[j];
            win_ready = true;
        }

        idx_t i = cached_len;                     /* compute a(i)   */
        seq_t val = (seq_t)45 * sum_win - (seq_t)1035 * cache[i - 46];
        cache[i] = val;

        /* update sliding window: drop a[i-45], add a[i] */
        sum_win += val;
        sum_win -= cache[i - 45];

        cached_len++;
    }

    return cache[n];
}
