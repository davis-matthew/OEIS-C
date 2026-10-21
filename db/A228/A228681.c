/*
 * A228681  Number of n X 6 binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 6 binary arrays with no two 1’s adjacent
 *            horizontally, diagonally or anti‑diagonally. The count is
 *            obtained by a simple DP over the 21 admissible 6‑bit row
 *            patterns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* width of the array (n × 6) */
#define WIDTH 6
/* number of admissible row patterns (no two horizontally adjacent 1’s) */
#define NUM_STATES 21

/* tables generated once on first call */
static int masks[NUM_STATES];
static unsigned char trans[NUM_STATES][NUM_STATES];
static bool initialized = false;

/* initialise masks and transition table */
static void init_states(void)
{
    if (initialized) return;

    /* collect all 6‑bit masks with no adjacent 1’s */
    int cnt = 0;
    for (int m = 0; m < (1 << WIDTH); ++m) {
        if (m & (m << 1)) continue;          /* horizontal adjacency forbidden */
        masks[cnt++] = m;
    }

    /* build allowed‑transition table */
    for (int i = 0; i < NUM_STATES; ++i) {
        for (int j = 0; j < NUM_STATES; ++j) {
            int a = masks[i];
            int b = masks[j];
            /* forbid diagonal conflicts: (a<<1) and (a>>1) */
            trans[i][j] = !((b & (a << 1)) | (b & (a >> 1)));
        }
    }

    initialized = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 as safe default */

    init_states();

    /* dp[mask] = number of valid arrays up to current row ending with mask */
    seq_t dp[NUM_STATES];
    seq_t ndp[NUM_STATES];

    /* initial row: any admissible mask is allowed once */
    for (int i = 0; i < NUM_STATES; ++i) dp[i] = 1;

    /* iterate over remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        for (int j = 0; j < NUM_STATES; ++j) ndp[j] = 0;

        for (int i = 0; i < NUM_STATES; ++i) {
            if (dp[i] == 0) continue;         /* skip if no sequences reach this state */
            for (int j = 0; j < NUM_STATES; ++j) {
                if (trans[i][j]) ndp[j] += dp[i];
            }
        }

        for (int i = 0; i < NUM_STATES; ++i) dp[i] = ndp[i];
    }

    /* total number of arrays is the sum over all ending states */
    seq_t total = 0;
    for (int i = 0; i < NUM_STATES; ++i) total += dp[i];

    return total;
}
