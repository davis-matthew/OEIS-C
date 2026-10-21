/*
 * A209809  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having two
 *          distinct values, and new values 0..3 introduced in row major
 *          order.
 * https://oeis.org/A209809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by counting (n+1)x4 arrays over 0..3 satisfying
 *            the “two distinct values in every 2×2 subblock” condition and
 *            the row‑major labelling rule. The computation is a dynamic
 *            program over row‑by‑row states. It is valid for all n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --- constants -------------------------------------------------------- */
#define NUM_COLS 4
#define NUM_VALUES 4
#define PATTERNS (NUM_VALUES * NUM_VALUES * NUM_VALUES * NUM_VALUES) /* 81 */
#define STATES  (PATTERNS * NUM_VALUES) /* prev_row (81) × current max (4) = 324 */
#define MAX_OUT 81   /* maximal outgoing edges from a state */

/* --- global pre‑computed tables -------------------------------------- */
static int pat_vals[PATTERNS][NUM_COLS];          /* pattern id -> 4 values */
static int trans_dst[STATES][MAX_OUT];           /* transitions from a state */
static uint8_t trans_cnt[STATES];                /* number of transitions per state */

/* --------------------------------------------------------------------- */
static void build_tables(void)
{
    static bool built = false;
    if (built) return;
    built = true;

    /* Enumerate all row patterns */
    for (int id = 0; id < PATTERNS; ++id) {
        int t = id;
        for (int i = NUM_COLS - 1; i >= 0; --i) {
            pat_vals[id][i] = t % NUM_VALUES;
            t /= NUM_VALUES;
        }
    }

    /* Build transitions for every (prev_pattern, cur_max) state */
    for (int p = 0; p < PATTERNS; ++p)                /* previous row pattern   */
    for (int m = 0; m < NUM_VALUES; ++m) {             /* current max value      */
        int state = p * NUM_VALUES + m;
        trans_cnt[state] = 0;

        const int *prev = pat_vals[p];

        for (int q = 0; q < PATTERNS; ++q) {           /* candidate new row     */
            const int *curr = pat_vals[q];
            bool ok = true;

            /* check 2×2 blocks with the previous row */
            for (int i = 0; i < NUM_COLS - 1; ++i) {
                int mask = (1 << prev[i]) | (1 << prev[i + 1]) |
                           (1 << curr[i]) | (1 << curr[i + 1]);
                if (__builtin_popcount(mask) != 2) { ok = false; break; }
            }
            if (!ok) continue;

            /* simulate labeling rule across the new row */
            int cur = m;
            for (int j = 0; j < NUM_COLS; ++j) {
                int val = curr[j];
                if (val > cur + 1 || val > 3) { ok = false; break; }
                if (val == cur + 1) cur++;
            }
            if (!ok) continue;

            int new_m = cur;
            int dest = q * NUM_VALUES + new_m;
            trans_dst[state][trans_cnt[state]++] = dest;
        }
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 1 ; we need n+1 rows */
    if (n == 0) return 0;   /* undefined for the sequence, but harmless */

    build_tables();

    const size_t total_rows = (size_t)n + 1;   /* number of rows in the table */

    static seq_t dp[STATES];
    static seq_t next[STATES];

    /* --- initialise with the first row -------------------------------- */
    for (size_t i = 0; i < STATES; ++i) dp[i] = 0;

    for (int q = 0; q < PATTERNS; ++q) {
        const int *row = pat_vals[q];
        if (row[0] != 0) continue;          /* first cell must be 0 */

        int cur = -1;
        bool ok = true;
        for (int j = 0; j < NUM_COLS; ++j) {
            int val = row[j];
            if (val > cur + 1 || val > 3) { ok = false; break; }
            if (val == cur + 1) cur++;
        }
        if (!ok) continue;

        int new_m = cur;
        int state = q * NUM_VALUES + new_m;
        dp[state] += (seq_t)1;               /* one way to finish the first row */
    }

    /* --- process subsequent rows ------------------------------------- */
    for (size_t step = 1; step < total_rows; ++step) {
        for (size_t i = 0; i < STATES; ++i) next[i] = 0;

        for (size_t i = 0; i < STATES; ++i) {
            seq_t val = dp[i];
            if (val == 0) continue;

            uint8_t c = trans_cnt[i];
            for (uint8_t k = 0; k < c; ++k) {
                int d = trans_dst[i][k];
                next[d] += val;
            }
        }
        for (size_t i = 0; i < STATES; ++i) dp[i] = next[i];
    }

    /* --- sum all final states ---------------------------------------- */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i) total += dp[i];

    return total;
}
