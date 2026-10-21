/*
 * A091890  Number of partitions of n into sums of exactly three distinct
 *          powers of 2.
 * https://oeis.org/A091890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute the number of partitions of n into parts, each part
 *            being the sum of exactly three distinct powers of 2.
 *            Equivalently, count the unordered partitions of n into numbers
 *            that have Hamming weight 3
 * Range:     u8 n <= 86, u16 n <= 184, u32 n <= 478, u64 n <= 1478,
 *            u128 n <= 5434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute floor(log2(n)) for n >= 1. */
static idx_t floor_log2(idx_t n) {
    idx_t r = 0;
    while (n > 1) {
        n >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Minimal part is 2^0+2^1+2^2 = 7. */
    if (n < 7)
        return (seq_t)0;

    /* Generate all numbers <= n that have exactly three bits set. */
    idx_t maxexp = floor_log2(n);
    /* Rough upper bound on number of such numbers: C(maxexp+1,3). */
    size_t maxcoins = (maxexp + 1) * maxexp * (maxexp - 1) / 6 + 10;
    idx_t *coins = (idx_t *)malloc(maxcoins * sizeof(idx_t));
    size_t coin_cnt = 0;

    for (idx_t a = 0; a <= maxexp; ++a) {
        for (idx_t b = a + 1; b <= maxexp; ++b) {
            for (idx_t c = b + 1; c <= maxexp; ++c) {
                idx_t val = ((idx_t)1 << a) | ((idx_t)1 << b) | ((idx_t)1 << c);
                if (val <= n) {
                    coins[coin_cnt++] = val;
                } else {
                    /* Since bits are increasing, further c will only increase val */
                    break;
                }
            }
        }
    }

    /* DP array for partition counts. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    dp[0] = (seq_t)1; /* one way to make 0 */

    for (size_t i = 0; i < coin_cnt; ++i) {
        idx_t val = coins[i];
        for (idx_t s = val; s <= n; ++s) {
            dp[s] += dp[s - val];
        }
    }

    seq_t result = dp[n];

    free(coins);
    free(dp);
    return result;
}
