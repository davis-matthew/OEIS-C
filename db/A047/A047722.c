/*
 * A047722  Sum of 11 but no fewer nonzero fourth powers.
 * https://oeis.org/A047722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that cannot be represented as a sum of
 *            fewer than 11 non‑zero fourth powers
 * Range:     u8 n <= 18, u16 n <= 4935, u32 n <= 9511, u64 n <= 9511,
 *            u128 n <= 9511, big any n
 * Helpers:   pow4_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow4_limit.h"

/* ----------------------------------------------------------- static state ----------------------------------------------------------- */
static uint16_t *dp          = NULL;   /* dp[i] = minimal number of 4th powers for i   */
static size_t   dp_size      = 0;       /* allocated size of dp                         */
static size_t   built_limit  = 0;       /* highest index that has been computed         */

static size_t *coins        = NULL;   /* list of 4th powers <= current limit          */
static size_t  coin_cnt     = 0;        /* number of coins                              */
static size_t  coins_cap    = 0;        /* allocated capacity of coins                  */

static seq_t *cache         = NULL;   /* sequence values (indices where dp==11)        */
static size_t cache_len     = 0;        /* number of stored values                      */
static size_t cache_cap     = 0;        /* allocated capacity of cache                  */

/* ----------------------------------------------------------- ensure coin list ----------------------------------------------------------- */
static void ensure_coins(size_t limit)
{
    while (1) {
        size_t next_exp = coin_cnt + 1;          /* next exponent to test   */
        size_t val = pow4_limit(next_exp, limit);
        if (val > limit)
            break;

        if (coin_cnt == coins_cap) {
            size_t new_cap = (coins_cap == 0) ? 32 : coins_cap * 2;
            size_t *tmp = (size_t *)realloc(coins, new_cap * sizeof(size_t));
            if (!tmp)
                abort();
            coins = tmp;
            coins_cap = new_cap;
        }
        coins[coin_cnt++] = val;
    }
}

/* ----------------------------------------------------------- extend dynamic program ----------------------------------------------------------- */
static void extend_to(size_t new_limit)
{
    if (new_limit <= built_limit)
        return;

    /* Resize dp array */
    size_t new_dp_size = new_limit + 1;
    uint16_t *tmp = (uint16_t *)realloc(dp, new_dp_size * sizeof(uint16_t));
    if (!tmp && new_dp_size > 0)
        abort();
    dp = tmp;
    dp_size = new_dp_size;

    /* Ensure dp[0] is set */
    dp[0] = 0;

    /* Ensure we know all 4th powers up to new_limit */
    ensure_coins(new_limit);

    /* Compute new dp values */
    for (size_t i = built_limit + 1; i <= new_limit; ++i) {
        uint16_t best = UINT16_MAX;
        for (size_t c = 0; c < coin_cnt; ++c) {
            size_t coin = coins[c];
            if (coin > i)
                break;
            uint16_t prev = dp[i - coin];
            if (prev == UINT16_MAX)
                continue;
            uint16_t cand = prev + 1;
            if (cand < best)
                best = cand;
        }
        dp[i] = best;

        /* If minimal count is exactly 11, store in cache */
        if (best == 11) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmpcache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmpcache)
                    abort();
                cache = tmpcache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)i;
        }
    }

    built_limit = new_limit;
}

/* ----------------------------------------------------------- build until n terms ----------------------------------------------------------- */
static void build_until_n(size_t target_n)
{
    if (target_n <= cache_len)
        return;

    size_t limit = built_limit;
    while (cache_len < target_n) {
        size_t new_limit;
        if (limit == 0)
            new_limit = 1000;
        else
            new_limit = limit * 2;

        if (new_limit <= limit)
            new_limit = limit + 500;   /* safety step */

        extend_to(new_limit);
        limit = new_limit;
    }
}

/* ----------------------------------------------------------- public sequence function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* behaviour below offset is undefined */

    /* Grow cache to required size */
    build_until_n((size_t)n);

    return cache[(size_t)n - 1];
}
