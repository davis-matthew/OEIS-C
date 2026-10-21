/*
 * A207876  Number of nX5 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A207876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding horizontal 000 and
 *            101, and vertical 010 or 111, a(1)=13, a(2)=169,
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 50, u128 n <= 105,
 *            big any n
 * Helpers:   allowed_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/allowed_row.h"

/* Constants for the DP */
#define ROWS 5
#define K 13                 /* number of valid row patterns   */
#define S (K*K)              /* number of state pairs (p,q)    */

/* Storage for row patterns and transitions */
static uint8_t patterns[K];
static int next_ids[S][K];   /* maximum K successors per state   */
static int next_count[S];    /* actual number of successors */

/* Initialise the patterns and transitions (once) */
static void init(void) {
    static bool done = false;
    if (done) return;
    /* Enumerate all 5‑bit patterns and keep the allowed ones */
    int pat_cnt = 0;
    for (uint8_t pat = 0; pat < 32; ++pat) {
        if (allowed_row(pat)) {
            patterns[pat_cnt++] = pat;
        }
    }

    /* Build transition graph */
    for (int p_idx = 0; p_idx < K; ++p_idx) {
        uint8_t p_pat = patterns[p_idx];
        for (int q_idx = 0; q_idx < K; ++q_idx) {
            uint8_t q_pat = patterns[q_idx];
            int state = p_idx * K + q_idx; /* id of state (p,q) */
            int cnt = 0;
            for (int r_idx = 0; r_idx < K; ++r_idx) {
                uint8_t r_pat = patterns[r_idx];
                bool ok = true;
                for (int col = 0; col < ROWS; ++col) {
                    uint8_t b1 = (p_pat >> col) & 1;
                    uint8_t b2 = (q_pat >> col) & 1;
                    uint8_t b3 = (r_pat >> col) & 1;
                    if ((b1 == 0 && b2 == 1 && b3 == 0) ||
                        (b1 == 1 && b2 == 1 && b3 == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    next_ids[state][cnt++] = r_idx;
                }
            }
            next_count[state] = cnt;
        }
    }
    done = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic safety */
    seq_t N = (seq_t)n;

    /* Base cases */
    if (n == 0) return 0;          /* outside offset, not used */
    if (n == 1) return (seq_t)K;   /* 13 */
    if (n == 2) return (seq_t)K * (seq_t)K; /* 169 */

    /* Ensure tables are set up */
    init();

    /* DP arrays for state counts */
    static seq_t dp[S];
    static seq_t newdp[S];

    /* Initialise dp for length = 2 */
    for (int s = 0; s < S; ++s) dp[s] = 1;

    /* Iterate from length 3 up to n */
    for (idx_t step = 0; step < n - 2; ++step) {
        for (int s = 0; s < S; ++s) newdp[s] = 0;
        for (int s = 0; s < S; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int cnt = next_count[s];
            for (int t = 0; t < cnt; ++t) {
                int ns = s % K * K + next_ids[s][t]; /* (q,r) index */
                newdp[ns] += val;
            }
        }
        for (int s = 0; s < S; ++s) dp[s] = newdp[s];
    }

    /* Sum over all end states to get total number of arrays of size n */
    seq_t total = 0;
    for (int s = 0; s < S; ++s) total += dp[s];
    return total;
}
