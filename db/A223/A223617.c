/*
 * A223617  Number of n X 5 0..1 arrays with rows and columns unimodal.
 * https://oeis.org/A223617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 5 0..1 arrays with rows and columns
 *            unimodal. Computation is done by dynamic programming over the
 *            3‑state history of each column (0 = zeros so far, 1 = ones in
 *            progress, 2 = zeros after the peak). Each row must be one of the
 *            16 unimodal 5‑bit patterns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 17, u64 n <= 171, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */

static const int ROW_PATTERNS[16] = {
    0,  1,  3,  7, 15, 31,   /* 0…0 1…1 0…0 with the 1‑block at the end */
    2,  6, 14, 30,           /* 1‑block starts in column 1 or 2 */
    4, 12, 28,               /* 1‑block starts in column 2 or 3 */
    8, 24,                   /* 1‑block starts in column 3 or 4 */
   16                          /* 1‑block starts in column 4 */
};

#define NUM_STATES 243   /* 3^5 column state combinations */
#define PATTERNS   16

/* Transition table: TRANS[source_state][pattern_index] = destination_state
   or -1 if the transition is impossible (i.e. would place a 1 after a 2). */
static short TRANS[NUM_STATES][PATTERNS];
static bool TRANS_BUILT = false;

static void build_transitions(void)
{
    for (int src = 0; src < NUM_STATES; ++src) {
        /* Decode source state into per‑column states 0..2 */
        int tmp = src;
        int cur[5];
        for (int col = 0; col < 5; ++col) {
            cur[col] = tmp % 3;
            tmp /= 3;
        }
        for (int p = 0; p < PATTERNS; ++p) {
            int pattern = ROW_PATTERNS[p];
            int dst = 0;
            int mult = 1;
            bool ok = true;
            for (int col = 0; col < 5; ++col) {
                int bit = (pattern >> col) & 1;
                int new_val;
                if (cur[col] == 2) {
                    if (bit == 1) { ok = false; break; }
                    new_val = 2;          /* stays in after‑peak state */
                } else if (cur[col] == 1) {
                    new_val = (bit == 1) ? 1 : 2;  /* stay or go to after‑peak */
                } else { /* cur==0 */
                    new_val = (bit == 1) ? 1 : 0;  /* start peak or stay zeros */
                }
                dst += new_val * mult;
                mult *= 3;
            }
            TRANS[src][p] = ok ? (short)dst : -1;
        }
    }
    TRANS_BUILT = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Build transition table on first call */
    if (!TRANS_BUILT) build_transitions();

    /* DP arrays: dp[s] = number of ways to reach state s after current row. */
    seq_t dp[NUM_STATES], next[NUM_STATES];
    for (int i = 0; i < NUM_STATES; ++i) dp[i] = 0;
    dp[0] = 1;  /* all columns start in state 0 */

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < NUM_STATES; ++i) next[i] = 0;
        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            for (int p = 0; p < PATTERNS; ++p) {
                short ns = TRANS[s][p];
                if (ns >= 0) next[ns] += ways;
            }
        }
        /* swap dp and next */
        for (int i = 0; i < NUM_STATES; ++i) dp[i] = next[i];
    }

    /* Sum over all possible final states */
    seq_t ans = 0;
    for (int i = 0; i < NUM_STATES; ++i) ans += dp[i];
    return ans;
}
