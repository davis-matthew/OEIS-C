/*
 * A229401  Number of nX7 0..2 arrays with top left element 0, horizontal
 *          differences mod 3 never 1, vertical differences mod 3 never -1,
 *          and antidiagonal differences never 0.
 * https://oeis.org/A229401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 arrays over {0,1,2} with top‑left element
 *            0, horizontal differences mod 3 never 1, vertical differences
 *            mod 3 never –1, antidiagonal differences never 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* maximum possible states (≤192) */
#define MAX_STATES 200
#define COLS 7
#define POW3_7 2187 /* 3^7 */

/* static data for the state space and transitions */
static idx_t state_count = 0;
static uint8_t state_vals[MAX_STATES][COLS];
static bool adj_mat[MAX_STATES][MAX_STATES];
static bool initialized = false;

/* initialise the state space and transition matrix once */
static void init_states(void)
{
    /* generate all row patterns satisfying the horizontal rule */
    for (idx_t num = 0; num < POW3_7; ++num) {
        uint8_t digits[COLS];
        idx_t tmp = num;
        for (int c = 0; c < COLS; ++c) {
            digits[c] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
        bool ok = true;
        for (int c = 0; c < COLS - 1; ++c) {
            if (digits[c + 1] == (uint8_t)((digits[c] + 1) % 3)) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        /* store the state */
        state_vals[state_count][0] = digits[0];
        state_vals[state_count][1] = digits[1];
        state_vals[state_count][2] = digits[2];
        state_vals[state_count][3] = digits[3];
        state_vals[state_count][4] = digits[4];
        state_vals[state_count][5] = digits[5];
        state_vals[state_count][6] = digits[6];
        ++state_count;
    }

    /* build adjacency matrix */
    for (idx_t i = 0; i < state_count; ++i) {
        for (idx_t j = 0; j < state_count; ++j) {
            bool ok = true;
            /* vertical difference: cannot be 2 mod 3 */
            for (int c = 0; c < COLS; ++c) {
                uint8_t a = state_vals[i][c];
                uint8_t b = state_vals[j][c];
                if (b == (uint8_t)((a + 2) % 3)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                adj_mat[i][j] = false;
                continue;
            }
            /* antidiagonal: adjacent cells cannot be equal */
            for (int c = 1; c < COLS; ++c) {
                if (state_vals[j][c - 1] == state_vals[i][c]) {
                    ok = false;
                    break;
                }
            }
            adj_mat[i][j] = ok;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_states();
        initialized = true;
    }

    const idx_t S = state_count;

    /* dp arrays: number of ways to reach each state after current row */
    static seq_t dp[MAX_STATES];
    static seq_t newdp[MAX_STATES];

    /* initialise the first row */
    for (idx_t i = 0; i < S; ++i) dp[i] = 0;
    for (idx_t i = 0; i < S; ++i) {
        if (state_vals[i][0] == 0) dp[i] = 1;  /* top‑left fixed to 0 */
    }

    /* if only one row */
    if (n == 1) {
        seq_t ans = 0;
        for (idx_t i = 0; i < S; ++i) ans += dp[i];
        return ans;
    }

    /* iterate over remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        for (idx_t j = 0; j < S; ++j) newdp[j] = 0;
        for (idx_t i = 0; i < S; ++i) {
            if (dp[i] == 0) continue;
            for (idx_t j = 0; j < S; ++j) {
                if (!adj_mat[i][j]) continue;
                newdp[j] = newdp[j] + dp[i];
            }
        }
        for (idx_t j = 0; j < S; ++j) dp[j] = newdp[j];
    }

    /* sum all ways for n rows */
    seq_t ans = 0;
    for (idx_t i = 0; i < S; ++i) ans += dp[i];
    return ans;
}
