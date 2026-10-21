/*
 * A239336  Number of nX4 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or three plus the sum of elements
 *          above it, modulo 4.
 * https://oeis.org/A239336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239336: Number of n×4 arrays with entries 0..3 such that
   each element is not equal to (1 + sum of elements to its left) mod 4
   nor (3 + sum of elements above it) mod 4. */
/* This implementation precomputes a 256×256 transition matrix
   for the column-sum states and then performs a standard
   dynamic-programming DP over rows, which runs in O(n·256²) time
   and uses only a few kilobytes of memory.  The transition counts
   are small (≤256) so multiplication by an int is efficient. */
#include <stdint.h>
#include <stddef.h>

/* Transition data for each previous state:
   trans_len[s]       – number of reachable states from s
   trans_to[s][k]     – the k‑th reachable next state
   trans_cnt[s][k]    – how many rows produce that transition  (≤256) */
static uint16_t trans_len[256];
static uint16_t trans_to[256][256];
static uint16_t trans_cnt[256][256];
static int precomputed = 0;

/* Helper to build the transition table once */
static void build_transitions(void)
{
    for (int s = 0; s < 256; ++s) {
        uint16_t cnt[256] = {0};
        for (int r = 0; r < 256; ++r) {
            int left = 0;
            int new_state = 0;
            int ok = 1;
            for (int j = 0; j < 4; ++j) {
                int a = (r >> (2 * j)) & 3;            /* value in column j */
                int s_j = (s >> (2 * j)) & 3;          /* current column sum mod4 */
                int forb1 = (left + 1) & 3;            /* 1 + left sum  */
                int forb2 = (s_j + 3) & 3;            /* 3 + sum above   */
                if (a == forb1 || a == forb2) {       /* violates constraint */
                    ok = 0;
                    break;
                }
                left = (left + a) & 3;                 /* left sum for next column */
                new_state |= ((s_j + a) & 3) << (2 * j); /* new column sums */
            }
            if (ok) {
                ++cnt[new_state];
            }
        }
        /* Store the non-zero transitions */
        uint16_t len = 0;
        for (int ns = 0; ns < 256; ++ns) {
            if (cnt[ns]) {
                trans_to[s][len] = (uint16_t)ns;
                trans_cnt[s][len] = (uint16_t)cnt[ns];
                ++len;
            }
        }
        trans_len[s] = len;
    }
    precomputed = 1;
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (!precomputed) {
        build_transitions();
    }

    /* DP vectors: f holds counts for height i, g for height i+1 */
    static seq_t f[256];
    static seq_t g[256];

    /* Initialize for row 0: column sums are all zero (state 0) */
    for (int i = 0; i < 256; ++i) f[i] = 0;
    f[0] = 1;

    for (idx_t row = 0; row < n; ++row) {
        /* Zero out next row counts */
        for (int i = 0; i < 256; ++i) g[i] = 0;

        /* Iterate over previous states with non‑zero counts */
        for (int s = 0; s < 256; ++s) {
            seq_t cur = f[s];
            if (cur == 0) continue;
            uint16_t len = trans_len[s];
            for (uint16_t k = 0; k < len; ++k) {
                int ns = trans_to[s][k];
                seq_t add = cur * (seq_t)trans_cnt[s][k];
                g[ns] += add;
            }
        }

        /* Copy g into f for the next iteration */
        for (int i = 0; i < 256; ++i) f[i] = g[i];
    }

    /* Sum over all final states */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) {
        result += f[i];
    }
    return result;
}
