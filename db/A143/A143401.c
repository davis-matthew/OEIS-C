/*
 * A143401  Expansion of x^k/Product_{t=k..2k} (1-tx) for k=6.
 * https://oeis.org/A143401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in x^6/∏_{t=6}^{12} (1-tx)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = 0 for n<6 and for n≥6 a(n) = [_x^{n-6}] 1/∏_{t=6}^{12} (1-tx) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp = NULL;          /* coefficients of 1/∏_{t=6}^{12} (1-tx) */
static idx_t dp_len = 0;          /* number of entries in dp (ndp = max m + 1) */

static seq_t *cache = NULL;       /* cached values a(0)…a(n_max) */
static idx_t cache_len = 0;       /* number of cached a(i) that are valid */

/* recompute dp for all m up to new_m (new_m ≥ dp_len-1) */
static void recompute_dp(idx_t new_m)
{
    /* free old dp buffer if present */
    if (dp) {
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
        dp = NULL;
        dp_len = 0;
    }

    idx_t len = new_m + 1;   /* we need coefficients for 0 … new_m inclusive */

    seq_t *old_arr;
    seq_t *new_arr;

#ifdef __cplusplus
    old_arr = new seq_t[len];
    new_arr = new seq_t[len];
#else
    old_arr = (seq_t*)malloc(sizeof(seq_t) * len);
    new_arr = (seq_t*)malloc(sizeof(seq_t) * len);
#endif

    /* start with the polynomial 1 */
    old_arr[0] = 1;
    for (idx_t i = 1; i < len; ++i)
        old_arr[i] = 0;

    const uint64_t t_vals[7] = {6, 7, 8, 9, 10, 11, 12};

    for (int k = 0; k < 7; ++k) {
        uint64_t t = t_vals[k];
        new_arr[0] = old_arr[0];
        for (idx_t i = 1; i < len; ++i) {
            new_arr[i] = old_arr[i];
            new_arr[i] += (seq_t)t * new_arr[i-1];
        }
        /* swap buffers for the next factor */
        seq_t *tmp = old_arr;
        old_arr = new_arr;
        new_arr = tmp;
    }

    /* now old_arr holds the final coefficients */
    dp = old_arr;
    dp_len = len;

    /* free the auxiliary buffer that is no longer needed */
#ifdef __cplusplus
    delete[] new_arr;
#else
    free(new_arr);
#endif
}

/* ensure cache array can hold at least new_len entries */
static void ensure_cache_size(idx_t new_len)
{
    if (new_len <= cache_len)
        return;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_len];
#else
    new_cache = (seq_t*)malloc(sizeof(seq_t) * new_len);
#endif

    /* copy existing cached values */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* free old cache */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* for n < 6 the coefficients are zero */
    if (n < 6)
        return 0;

    idx_t need_m = n - 6;   /* we need dp[0…need_m] */

    /* recompute dp if we have no data or we need more coefficients */
    if (need_m >= dp_len)
        recompute_dp(need_m);

    /* remember old cache length before possibly resizing */
    idx_t old_cache_len = cache_len;

    /* make sure cache is large enough to hold indices up to n */
    if (n >= cache_len)
        ensure_cache_size(n + 1);

    /* fill any missing cached values */
    for (idx_t i = old_cache_len; i <= n; ++i) {
        if (i < 6)
            cache[i] = 0;
        else
            cache[i] = dp[i - 6];
    }

    /* update cache_len to reflect up to n inclusive */
    cache_len = n + 1;

    return cache[n];
}
