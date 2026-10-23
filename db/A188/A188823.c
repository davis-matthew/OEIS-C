/*
 * A188823  Number of n X 8 binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by counting n‑row binary arrays of width 8 that
 *            avoid the diagonal pattern 0 1 (down‑right) and 0 1 (down‑left).
 *            This is done via dynamic programming over the 256 possible rows
 * Range:     u8 none, u16 n <= 5, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary standard includes for any integer types and bool. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute allowable transitions once. */
    /* Each value in 'next_masks' holds an array of subsequent rows
       that can follow the current row without violating the pattern. */
    static bool initialized = false;
    static uint8_t next_masks[256][256]; /* maximum 256 transitions per row */
    static uint16_t next_cnt[256];
    if (!initialized) {
        const uint8_t FULL_MASK = 0xFF;
        for (uint16_t prev = 0; prev < 256; ++prev) {
            uint8_t forbid = 0;
            for (int j = 0; j < 8; ++j) {
                if (((prev >> j) & 1) == 0) {
                    if (j > 0) forbid |= 1U << (j - 1);
                    if (j < 7) forbid |= 1U << (j + 1);
                }
            }
            uint8_t allow = (~forbid) & FULL_MASK;
            uint8_t sub = allow;
            uint16_t idx = 0;
            do {
                next_masks[prev][idx++] = sub;
                if (sub == 0) break;
                sub = (sub - 1) & allow;
            } while (true);
            next_cnt[prev] = idx;
        }
        initialized = true;
    }

    /* If only one row, every pattern is allowed: 2^(8) = 256 ways. */
    if (n == 1) {
        return (seq_t)256;
    }

    /* DP arrays: dp[mask] holds number of ways to reach current last row mask. */
    static seq_t dp[256];
    static seq_t next_dp[256];
    for (int i = 0; i < 256; ++i) {
        dp[i] = (seq_t)1;        /* first row can be any of the 256 masks */
    }

    /* Iterate through rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero the next layer */
        for (int i = 0; i < 256; ++i) {
            next_dp[i] = (seq_t)0;
        }
        /* Transition */
        for (uint16_t prev = 0; prev < 256; ++prev) {
            seq_t val = dp[prev];
            if (val == 0) continue;
            uint16_t cnt = next_cnt[prev];
            for (uint16_t k = 0; k < cnt; ++k) {
                uint8_t nxt = next_masks[prev][k];
                next_dp[nxt] += val;
            }
        }
        /* Copy next_dp back into dp for next iteration */
        for (int i = 0; i < 256; ++i) {
            dp[i] = next_dp[i];
        }
    }

    /* Sum over all possible final row masks */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) {
        result += dp[i];
    }
    return result;
}
