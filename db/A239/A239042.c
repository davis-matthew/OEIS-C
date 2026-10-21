/*
 * A239042  Number of nX3 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays (entries 0..3) with no cell equal to
 *            1 plus the sum of elements to its left or the sum of elements
 *            above it, modulo 4. Computation: dynamic programming over the
 *            column‑sum states (each column holds a running sum modulo 4).
 *            For each row we precompute how many rows trans...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"

/* Number of states: each of three columns can have a sum 0..3. */
#define STATE_CNT 64

/* Transition counts: T[s][d] = number of rows that take state s → d. */
static uint8_t trans_cnt[STATE_CNT][STATE_CNT];
static int trans_initialized = 0;

/* Precompute the transition matrix once. */
static void init_trans(void)
{
    if (trans_initialized) return;
    for (unsigned prev = 0; prev < STATE_CNT; ++prev) {
        unsigned p0 = prev & 0x3U;
        unsigned p1 = (prev >> 2) & 0x3U;
        unsigned p2 = (prev >> 4) & 0x3U;
        for (unsigned a = 0; a < 4U; ++a) {
            /* a cannot be 1 and cannot be 1+prev0 (mod 4). */
            if (a == 1U || a == ((1U + p0) & 0x3U)) continue;
            for (unsigned b = 0; b < 4U; ++b) {
                /* b cannot be 1+a and cannot be 1+prev1 (mod 4). */
                if (b == ((1U + a) & 0x3U) ||
                    b == ((1U + p1) & 0x3U)) continue;
                for (unsigned c = 0; c < 4U; ++c) {
                    /* c cannot be 1+(a+b) and cannot be 1+prev2 (mod 4). */
                    unsigned left = (a + b) & 0x3U;
                    if (c == ((1U + left) & 0x3U) ||
                        c == ((1U + p2) & 0x3U)) continue;
                    unsigned d0 = (p0 + a) & 0x3U;
                    unsigned d1 = (p1 + b) & 0x3U;
                    unsigned d2 = (p2 + c) & 0x3U;
                    unsigned dest = d0 | (d1 << 2) | (d2 << 4);
                    ++trans_cnt[prev][dest];
                }
            }
        }
    }
    trans_initialized = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;   /* (offset is 1, so a(0) is unused) */

    init_trans();

    /* Allocate DP vectors. */
    seq_t *dp   = (seq_t *)malloc(STATE_CNT * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(STATE_CNT * sizeof(seq_t));
    if (!dp || !next) { free(dp); free(next); return 0; }

    /* Start from state 0 (all column sums zero). */
    for (size_t i = 0; i < STATE_CNT; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t row = 0; row < n; ++row) {
        /* Zero next vector. */
        for (size_t i = 0; i < STATE_CNT; ++i) next[i] = 0;

        /* Transition step. */
        for (size_t s = 0; s < STATE_CNT; ++s) {
            if (dp[s] == 0) continue;
            for (size_t d = 0; d < STATE_CNT; ++d) {
                uint8_t cnt = trans_cnt[s][d];
                if (cnt == 0) continue;
                seq_t prod = seq_mul(dp[s], (seq_t)cnt);
                next[d] += prod;
            }
        }

        /* Copy next into dp for the next iteration. */
        for (size_t i = 0; i < STATE_CNT; ++i) dp[i] = next[i];
    }

    /* Sum all reachable states to get the total number of arrays. */
    seq_t result = 0;
    for (size_t i = 0; i < STATE_CNT; ++i) result += dp[i];

    free(dp);
    free(next);
    return result;
}
