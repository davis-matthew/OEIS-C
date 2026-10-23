/*
 * A131361  Number of partitions of n into repdigits of digits of n.
 * https://oeis.org/A131361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of partitions of n into repdigits of the non‑zero
 *            digits appearing in the decimal representation of n. a(0) = 1
 * Range:     u8 n <= 101, u16 n <= 212, u32 n <= 1233, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case: only the empty partition of 0. */
    if (n == 0)
        return (seq_t)1;

    /* Determine which non‑zero digits appear in n. */
    bool digit_present[10] = { false };
    idx_t tmp = n;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (d != 0)
            digit_present[d] = true;
        tmp /= 10;
    }

    /* Build the list of repdigit coin values ≤ n (using only
       the present non‑zero digits).  Coins are stored as idx_t
       because their value never exceeds n (an idx_t). */
    idx_t *coins = NULL;
    size_t coin_cnt   = 0;
    size_t coin_cap   = 0;

    for (int d = 1; d <= 9; ++d) {
        if (!digit_present[d])
            continue;

        /* Generate repdigits of digit d: d, dd, ddd, ... */
        idx_t value = d;
        while (value <= n) {
            if (coin_cnt == coin_cap) {
                coin_cap = coin_cap ? coin_cap * 2 : 16;
                idx_t *tmp_ptr = (idx_t *)realloc(coins, coin_cap * sizeof(idx_t));
                if (!tmp_ptr) {
                    /* Allocation failure; return 0 as failure code. */
                    free(coins);
                    return (seq_t)0;
                }
                coins = tmp_ptr;
            }
            coins[coin_cnt++] = value;

            /* Compute the next repdigit safely without overflow. */
            if (value > (n - (idx_t)d) / 10)  /* would exceed n on next step */
                break;
            value = value * 10 + (idx_t)d;
        }
    }

    /* Dynamic programming: ways[i] = number of partitions of i. */
    size_t len = (size_t)n + 1;
    seq_t *ways = (seq_t *)calloc(len, sizeof(seq_t));
    if (!ways) {
        free(coins);
        return (seq_t)0;
    }
    ways[0] = (seq_t)1;

    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t i = coin; i <= n; ++i) {
            ways[i] += ways[i - coin];
        }
    }

    seq_t result = ways[n];
    free(ways);
    free(coins);
    return result;
}
