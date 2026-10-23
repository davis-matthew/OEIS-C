/*
 * A179350  Number of permutations of 1..n with i-8<=p(i)<=i+3.
 * https://oeis.org/A179350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations p of 1..n with i-8 <= p(i) <= i+3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define B 12                          /* window size (8 left + 3 right + 1) */
#define MASK_FULL ((1u << B) - 1)

seq_t a(idx_t n) {
    if (n <= 0) return (seq_t)0;

    /* local DP tables */
    static seq_t dp[1 << B];
    static seq_t next_dp[1 << B];
    /* zero initialize dp */
    for (size_t i = 0; i < (1u << B); ++i) dp[i] = (seq_t)0;

    /* initial mask for position i = 1 */
    int64_t i = 1;
    unsigned int init_mask = 0;
    for (int j = 0; j < B; ++j) {
        int64_t v = i - 8 + j;           /* value corresponding to bit j */
        if (v < 1 || v > (int64_t)n)          /* out of real range => already used */
            init_mask |= (1u << j);
    }
    dp[init_mask] = (seq_t)1;

    /* DP over positions */
    for (int64_t pos = 1; pos <= (int64_t)n; ++pos) {
        /* assignment step: choose an unused valid value */
        for (size_t m = 0; m < (1u << B); ++m) next_dp[m] = (seq_t)0;
        for (size_t mask = 0; mask < (1u << B); ++mask) {
            seq_t ways = dp[mask];
            if (ways == (seq_t)0) continue;
            for (int j = 0; j < B; ++j) {
                int64_t v = pos - 8 + j;
                if (v < 1 || v > (int64_t)n) continue;   /* not a real element */
                if (!(mask & (1u << j))) {               /* not used yet */
                    unsigned int new_mask = mask | (1u << j);
                    next_dp[new_mask] += ways;
                }
            }
        }

        /* shift step: move window forward */
        for (size_t m = 0; m < (1u << B); ++m) dp[m] = (seq_t)0;
        for (size_t mask = 0; mask < (1u << B); ++mask) {
            seq_t ways = next_dp[mask];
            if (ways == (seq_t)0) continue;
            /* the leftmost value (pos-8) must have been used */
            if ((mask & 1u) == 0u) continue;
            unsigned int shifted = mask >> 1;                /* drop leftmost */
            unsigned int high_bit = ((pos + 4) <= (int64_t)n) ? 0u : 1u;
            shifted |= (high_bit << (B - 1));
            dp[shifted] += ways;
        }
    }

    /* after processing all positions, check that all real values have been used */
    unsigned int required_mask = 0;
    for (int j = 0; j < B; ++j) {
        int64_t v = (int64_t)(n + 1) - 8 + j;   /* value at new window's bit j */
        if (v >= 1 && v <= (int64_t)n)
            required_mask |= (1u << j);
    }

    seq_t result = (seq_t)0;
    for (size_t mask = 0; mask < (1u << B); ++mask) {
        seq_t ways = dp[mask];
        if (ways == (seq_t)0) continue;
        if ((mask & required_mask) == required_mask)
            result += ways;
    }

    return result;
}
