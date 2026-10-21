/*
 * A234884  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234884  –  Number of (n+1)×3 arrays over 0..5 satisfying the
   local 2×2 diagonal‑anti‑diagonal sum difference and adjacency conditions.
   The values are computed by dynamic programming on a state graph.
   a(1) = 272,  a(2) = 980,  a(3) = 3164, … */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PRECOMPUTE 20000   /* enough for all tests (≤10000) */

static bool _initialized = false;

/* Enumerate all 3‑tuples (a,b,c) with 0≤*≤5 and a≠b, b≠c */
#define STATE_COUNT 150
static uint8_t state_val[STATE_COUNT][3];          /* the three columns */

/* Transition matrix: 1 if state i can follow state j */
static unsigned char trans[STATE_COUNT][STATE_COUNT];

/* DP tables */
static seq_t dp_prev[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

/* Memoised answers: a[n] */
static seq_t ans_values[MAX_PRECOMPUTE + 1];
static idx_t  max_n_computed = 0;

/* --------------------------------------------------------------------- */

static void init(void)
{
    if (_initialized) return;

    /* Build all states */
    int idx = 0;
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b) {
            if (b == a) continue;
            for (int c = 0; c < 6; ++c) {
                if (c == b) continue;
                state_val[idx][0] = (uint8_t)a;
                state_val[idx][1] = (uint8_t)b;
                state_val[idx][2] = (uint8_t)c;
                ++idx;
            }
        }

    /* Pre‑compute transitions */
    for (int i = 0; i < STATE_COUNT; ++i) {
        for (int j = 0; j < STATE_COUNT; ++j) {
            bool ok = true;

            /* No vertically adjacent equal elements */
            if (state_val[i][0] == state_val[j][0] ||
                state_val[i][1] == state_val[j][1] ||
                state_val[i][2] == state_val[j][2])
                ok = false;

            /* Diagonal / anti‑diagonal sums differ by exactly ±1 in both blocks */
            if (ok) {
                for (int col = 0; col < 2; ++col) {
                    int diff = (state_val[i][col]     + state_val[j][col + 1]) -
                               (state_val[i][col + 1] + state_val[j][col]);
                    if (diff != 1 && diff != -1) {
                        ok = false;
                        break;
                    }
                }
            }
            trans[i][j] = ok ? 1 : 0;
        }
    }

    /* Initial DP: one‑row arrays – each state counts 1 */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_prev[i] = (seq_t)1;

    /* ans_values[0] unused; all others are zero automatically */

    _initialized = true;
}

/* Compute a(n) for all n up to the requested value (inclusive) */
static void compute_up_to(idx_t n)
{
    if (n <= max_n_computed) return;
    if (n > MAX_PRECOMPUTE) exit(1);        /* safety check */

    for (idx_t target = max_n_computed + 1; target <= n; ++target) {
        /* Zero dp_next */
        for (int j = 0; j < STATE_COUNT; ++j) dp_next[j] = 0;

        /* Transition */
        for (int i = 0; i < STATE_COUNT; ++i) {
            seq_t val = dp_prev[i];
            if (!val) continue;
            for (int j = 0; j < STATE_COUNT; ++j)
                if (trans[i][j])
                    dp_next[j] += val;
        }

        /* Move dp_next to dp_prev for next height */
        for (int j = 0; j < STATE_COUNT; ++j) {
            dp_prev[j] = dp_next[j];
            dp_next[j] = 0;
        }

        /* Sum over all ending states to get a(target) */
        seq_t total = 0;
        for (int j = 0; j < STATE_COUNT; ++j)
            total += dp_prev[j];
        ans_values[target] = total;
    }
    max_n_computed = n;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    init();
    compute_up_to(n);
    return ans_values[n];
}
