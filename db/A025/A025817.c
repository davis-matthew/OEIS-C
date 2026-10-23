/*
 * A025817  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A025817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A025817 – Number of partitions of n into parts 2,7,9
 * Formula: a(n) is the number of non‑negative integer solutions of
 *         2*x2 + 7*x7 + 9*x9 = n.
 * Implementation uses a classic coin‑change DP where the coins (2,7,9)
 * are processed in outer loop, guaranteeing order‑independent counts.
 */

#include <stdlib.h>

static const idx_t COINS[3] = { 2, 7, 9 };

/* Memoisation table: memo[n] holds a(n) for 0 ≤ n < memo_cap. */
static seq_t *memo = NULL;
static size_t memo_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure the table is large enough.  When expanding we recompute the
     * entire DP up to the new size; this is sufficiently fast for
     * n ≤ 10000 and guarantees correctness regardless of call order.
     */
    const size_t new_cap = (size_t)(n + 1);
    if (memo_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Recompute the DP from scratch for all indices < new_cap. */
    memo[0] = (seq_t)1;
    for (size_t i = 1; i < memo_cap; ++i)
        memo[i] = 0;

    for (int ci = 0; ci < 3; ++ci) {
        const idx_t coin = COINS[ci];
        for (idx_t i = coin; i < new_cap; ++i)
            memo[i] += memo[i - coin];
    }

    return memo[n];
}
