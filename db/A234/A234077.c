/*
 * A234077  Number of (n+1)X(2+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 10.
 * https://oeis.org/A234077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234077: Number of (n+1)×3 arrays with entries 0…4
   such that for every 2×2 subblock the sum of the absolute values of
   the six edge‑and‑diagonal differences equals 10.
   Computation is by dynamic programming over the 125 possible 3‑cell rows;
   the transition graph is pre‑computed once. */

#include <stdint.h>
#include <stdbool.h>

/* There are 5³ = 125 possible rows of length 3. */
#define ROWS 125

/* adjacency lists: succ[i][k] holds the next row that can follow row i;
   succ_cnt[i] is the number of such rows. */
static int  succ[ROWS][ROWS];
static uint8_t succ_cnt[ROWS];
static bool adj_init = false;

/* Test whether two rows a (top) and b (bottom) can form a valid 2×2
   block between columns 0–1 and 1–2. */
static inline bool valid_transition(int a, int b)
{
    int a0 = a % 5;
    int a1 = (a / 5) % 5;
    int a2 = a / 25;      /* 0…4 */

    int b0 = b % 5;
    int b1 = (b / 5) % 5;
    int b2 = b / 25;

    /* Check the two subblocks separately. */
    for (int j = 0; j < 2; ++j) {
        int ax = (j == 0) ? a0 : a1;
        int ay = (j == 0) ? a1 : a2;
        int bx = (j == 0) ? b0 : b1;
        int by = (j == 0) ? b1 : b2;

        int sum = 0, d;

        d = ax - bx; if (d < 0) d = -d; sum += d;       /* vertical   */
        d = ay - by; if (d < 0) d = -d; sum += d;       /* vertical   */
        d = ax - ay; if (d < 0) d = -d; sum += d;       /* horizontal */
        d = bx - by; if (d < 0) d = -d; sum += d;       /* horizontal */
        d = ax - by; if (d < 0) d = -d; sum += d;       /* diagonal   */
        d = ay - bx; if (d < 0) d = -d; sum += d;       /* diagonal   */

        if (sum != 10)
            return false;
    }
    return true;
}

/* Build adjacency lists once. */
static void init_adjacency(void)
{
    if (adj_init)
        return;

    for (int i = 0; i < ROWS; ++i)
        succ_cnt[i] = 0;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            if (valid_transition(i, j)) {
                succ[i][succ_cnt[i]] = j;
                ++succ_cnt[i];
            }
        }
    }
    adj_init = true;
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. For n<1 we return 0. */
    if (n < 1)
        return 0;

    init_adjacency();

    seq_t dp[ROWS], ndp[ROWS];
    for (int i = 0; i < ROWS; ++i)
        dp[i] = 1;          /* any first row is allowed */

    /* We need n transitions to build n+1 rows. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ROWS; ++i)
            ndp[i] = 0;

        for (int prev = 0; prev < ROWS; ++prev) {
            if (dp[prev] == 0)
                continue;
            int cnt = succ_cnt[prev];
            for (int k = 0; k < cnt; ++k) {
                int next = succ[prev][k];
                ndp[next] += dp[prev];
            }
        }
        for (int i = 0; i < ROWS; ++i)
            dp[i] = ndp[i];
    }

    seq_t total = 0;
    for (int i = 0; i < ROWS; ++i)
        total += dp[i];
    return total;
}
