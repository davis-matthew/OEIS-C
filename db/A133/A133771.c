/*
 * A133771  Number of runs (of equal bits) in the maximal Lucas binary
 *          (A130311) representation of n.
 * https://oeis.org/A133771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of runs in the binary string obtained by
 *            choosing, among all subsets of Lucas numbers that sum to n, the
 *            one whose binary mask (sum of 2^i for selected indices i) is
 *            minimal
 * Range:     u8 n <= 46, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* find the index of the most significant set bit in x (-1 if x == 0) */
static inline int highest_bit(seq_t x) {
    int idx = -1;
    while (x) {
        ++idx;
        x >>= 1;
    }
    return idx;
}

/* number of runs in binary representation of the mask */
static inline seq_t count_runs(seq_t mask) {
    int h = highest_bit(mask);
    if (h < 0) /* should never happen for n >= 1 */
        return 0;
    int runs = 1;
    int prev = (int)((mask >> h) & (seq_t)1);
    for (int i = h - 1; i >= 0; --i) {
        int cur = (int)((mask >> i) & (seq_t)1);
        if (cur != prev) {
            ++runs;
            prev = cur;
        }
    }
    return (seq_t)runs;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) /* out of offset; defined as 0 */
        return 0;

    /* build Lucas numbers up to n */
    seq_t lucas[128];
    int M = 0;
    lucas[M++] = (seq_t)2;          /* L0 */
    lucas[M++] = (seq_t)1;          /* L1 */
    while (true) {
        seq_t nxt = lucas[M - 1] + lucas[M - 2];
        if (nxt > (seq_t)n && M >= 2) break;
        lucas[M++] = nxt;
        if (M >= 128) break;        /* safety */
    }

    /* dynamic programming: dp[s] = minimal mask achieving sum s */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    bool *reach = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!dp || !reach) {          /* allocation failure */
        free(dp);
        free(reach);
        return 0;
    }
    dp[0] = 0;
    reach[0] = true;

    for (int i = 0; i < M; ++i) {
        seq_t weight = lucas[i];
        if (weight > (seq_t)n) continue;
        idx_t w_idx = (idx_t)weight;
        seq_t bit = ((seq_t)1 << i);

        for (idx_t s = n; s >= w_idx; --s) {
            if (!reach[s - w_idx]) {
                if (s == w_idx) break; /* avoid wrap‑around */
                continue;
            }
            seq_t cand = dp[s - w_idx] | bit;
            if (!reach[s] || cand < dp[s]) {
                dp[s] = cand;
                reach[s] = true;
            }
            if (s == w_idx) break; /* prevent unsigned underflow */
        }
    }

    seq_t bestMask = dp[n];
    free(dp);
    free(reach);

    return count_runs(bestMask);
}
