/*
 * A206881  Number of nX6 0..1 arrays avoiding 0 1 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×6 binary arrays avoiding a horizontal 010-pattern
 *            and a vertical 101-pattern
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global data prepared once on first call */
static int init_done = 0;
static int R = 0;                     /* number of valid rows */
static uint8_t *valid_rows = NULL;     /* array of valid row bitmasks */
static uint64_t *allowed_mask = NULL;  /* allowed next-row masks for each (prevprev,prev) pair */
static seq_t *dp = NULL;              /* DP array for current layer */
static seq_t *next_dp = NULL;          /* DP array for next layer */

/* Initialization: build valid rows and pre‑compute transition masks */
static void init(void)
{
    int patterns[64];
    int cnt = 0;
    for (int pat = 0; pat < 64; ++pat) {
        int ok = 1;
        for (int start = 0; start <= 3; ++start) {
            int b0 = (pat >> start)     & 1;
            int b1 = (pat >> (start+1)) & 1;
            int b2 = (pat >> (start+2)) & 1;
            if (b0 == 0 && b1 == 1 && b2 == 0) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            patterns[cnt++] = pat;
        }
    }
    R = cnt;
    valid_rows = (uint8_t*)malloc(R * sizeof(uint8_t));
    for (int i = 0; i < R; ++i) valid_rows[i] = (uint8_t)patterns[i];

    /* Pre‑compute allowed next‑row masks */
    size_t total_states = (size_t)R * R;
    allowed_mask = (uint64_t*)malloc(total_states * sizeof(uint64_t));
    for (int a = 0; a < R; ++a) {
        for (int b = 0; b < R; ++b) {
            uint64_t mask = 0;
            uint8_t pa = valid_rows[a];
            uint8_t pb = valid_rows[b];
            for (int c = 0; c < R; ++c) {
                uint8_t pc = valid_rows[c];
                /* forbid vertical 1‑0‑1 pattern: (pa & ~pb & pc) == 0 */
                if ((pa & (~pb) & pc) == 0) {
                    mask |= ((uint64_t)1 << c);
                }
            }
            allowed_mask[a * R + b] = mask;
        }
    }

    /* Allocate DP arrays */
    dp = (seq_t*)malloc(total_states * sizeof(seq_t));
    next_dp = (seq_t*)malloc(total_states * sizeof(seq_t));

    init_done = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!init_done) init();

    if (n == 1) {
        return (seq_t)R;
    }
    if (n == 2) {
        return (seq_t)R * (seq_t)R;
    }

    /* Initialize DP for the first two rows: every pair is allowed */
    size_t total_states = (size_t)R * R;
    for (size_t i = 0; i < total_states; ++i) dp[i] = 0;
    for (int a = 0; a < R; ++a) {
        for (int b = 0; b < R; ++b) {
            dp[a * R + b] = 1;
        }
    }

    idx_t steps = n - 2;
    for (idx_t step = 0; step < steps; ++step) {
        /* zero next layer */
        for (size_t i = 0; i < total_states; ++i) next_dp[i] = 0;
        /* transition */
        for (int a = 0; a < R; ++a) {
            for (int b = 0; b < R; ++b) {
                size_t idx = a * R + b;
                seq_t val = dp[idx];
                if (val == 0) continue;
                uint64_t mask = allowed_mask[idx];
                while (mask) {
                    uint64_t low = mask & -mask;          /* lowest set bit */
                    int c = __builtin_ctzll(mask);         /* its index */
                    size_t dst = b * R + c;
                    next_dp[dst] += val;
                    mask ^= low;
                }
            }
        }
        /* swap dp arrays */
        seq_t *tmp = dp;
        dp = next_dp;
        next_dp = tmp;
    }

    /* Sum all states to get the total number of arrays of size n×6 */
    seq_t total = 0;
    for (size_t i = 0; i < total_states; ++i) {
        total += dp[i];
    }
    return total;
}
