/*
 * A234533  Smallest number of hex numbers summing to n.
 * https://oeis.org/A234533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1494, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal number of centered hexagonal numbers summing to n
   using a dynamic programming approach. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* --- Static memoisation state ------------------------------------------------- */
    static seq_t *dp       = NULL;   /* dp[i] = minimal number of hex numbers for i */
    static size_t dp_size  = 0;      /* current allocated size of dp   */
    static seq_t *coins    = NULL;   /* list of centered hexagonal numbers */
    static size_t coin_cap = 0;      /* allocated capacity for coins */
    static size_t coin_cnt = 0;      /* number of stored coins            */
    static idx_t  max_k    = 0;      /* largest k used to generate coins   */

    if (n == 0) return 0;            /* safety for index < 1                 */

    /* --- Grow the dp table to cover n ------------------------------------------- */
    if (dp_size <= (size_t)n)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 */
        dp = tmp;

        /* initialise new entries – using only 1‑coin gives an upper bound */
        for (size_t i = dp_size; i < new_size; ++i)
            dp[i] = (seq_t)i;

        /* --- Generate new coin denominations (centered hexagonals) ---------------- */
        if (coins == NULL)
        {
            coin_cap = 64;
            coins = (seq_t*)malloc(coin_cap * sizeof(seq_t));
        }

        idx_t k;
        for (k = max_k + 1; ; ++k)
        {
            seq_t hex = (seq_t)3 * (seq_t)k * (seq_t)(k - 1) + 1;
            if (hex > (seq_t)n) break;

            if (coin_cnt == coin_cap)
            {
                size_t new_cap = coin_cap * 2;
                seq_t *old = (seq_t*)realloc(coins, new_cap * sizeof(seq_t));
                if (!old) return 0;
                coins   = old;
                coin_cap = new_cap;
            }
            coins[coin_cnt++] = hex;
        }
        max_k = k - 1;                 /* last k that produced a coin <= n */

        /* --- Dynamic‑programming update for new indices ------------------------ */
        for (size_t i = dp_size; i < new_size; ++i)
        {
            for (size_t c = 0; c < coin_cnt; ++c)
            {
                seq_t coin = coins[c];
                if (coin > (seq_t)i) break;

                seq_t diff = (seq_t)i - coin;
                size_t idx = (size_t)diff;
                seq_t candidate = dp[idx] + (seq_t)1;
                if (candidate < dp[i]) dp[i] = candidate;
            }
        }
        dp_size = new_size;
    }

    return dp[(size_t)n];
}
