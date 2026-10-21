/*
 * A229379  Number of nX7 0..2 arrays avoiding 11 horizontally, 22 vertically
 *          and 00 diagonally or antidiagonally.
 * https://oeis.org/A229379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 arrays over {0,1,2} avoiding horizontal 11,
 *            vertical 22 and diagonal or antidiagonal 00. Computation is done
 *            by a transfer‑matrix – dynamic programming over column states
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Max number of possible columns: 3^7 = 2187                      */
#define MAX_COLS 2187
#define ROWS 7

/* Global tables (initialised once)                                 */
static unsigned char  col_digit[MAX_COLS][ROWS];  /* decoded digits   */
static int            col_val[MAX_COLS];          /* integer value   */
static size_t        state_count;                 /* number of valid states */
static size_t       *succ_cnt;                    /* number of successors per state */
static int          **succ;                        /* list of successors per state */

/* Single‑time initialisation of the state tables                 */
static void init_once(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* ------------------------------------------------------------------
     *  Enumerate all 3^7 column patterns that satisfy the vertical 22 rule
     * ------------------------------------------------------------------ */
    state_count = 0;
    for (int v = 0; v < MAX_COLS; ++v) {
        int tmp = v;
        bool ok = true;
        for (int r = 0; r < ROWS; ++r) {
            col_digit[state_count][r] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
        for (int r = 0; r < ROWS - 1; ++r) {
            if (col_digit[state_count][r] == 2 &&
                col_digit[state_count][r+1] == 2) { ok = false; break; }
        }
        if (ok) {
            col_val[state_count] = v;
            ++state_count;
        }
    }

    /* ------------------------------------------------------------------
     *  Count successors for every state
     * ------------------------------------------------------------------ */
    succ_cnt = (size_t *)malloc(state_count * sizeof(size_t));
    succ     = (int **)malloc(state_count * sizeof(int *));
    for (size_t s = 0; s < state_count; ++s) {
        size_t cnt = 0;
        for (size_t t = 0; t < state_count; ++t) {
            bool ok = true;
            /* horizontal 11 rule */
            for (int r = 0; r < ROWS; ++r) {
                if (col_digit[s][r] == 1 && col_digit[t][r] == 1) { ok = false; break; }
            }
            if (!ok) continue;
            /* main diagonal 00 rule */
            for (int r = 0; r < ROWS-1; ++r) {
                if (col_digit[s][r] == 0 && col_digit[t][r+1] == 0) { ok = false; break; }
            }
            if (!ok) continue;
            /* antidiagonal 00 rule */
            for (int r = 0; r < ROWS-1; ++r) {
                if (col_digit[s][r+1] == 0 && col_digit[t][r] == 0) { ok = false; break; }
            }
            if (ok) ++cnt;
        }
        succ_cnt[s] = cnt;
    }

    /* ------------------------------------------------------------------
     *  Allocate and fill successor lists
     * ------------------------------------------------------------------ */
    for (size_t s = 0; s < state_count; ++s) {
        if (succ_cnt[s] == 0) { succ[s] = NULL; continue; }
        int *arr = (int *)malloc(succ_cnt[s] * sizeof(int));
        succ[s] = arr;
        size_t idx = 0;
        for (size_t t = 0; t < state_count; ++t) {
            bool ok = true;
            for (int r = 0; r < ROWS; ++r) {
                if (col_digit[s][r] == 1 && col_digit[t][r] == 1) { ok = false; break; }
            }
            if (!ok) continue;
            for (int r = 0; r < ROWS-1; ++r) {
                if (col_digit[s][r] == 0 && col_digit[t][r+1] == 0) { ok = false; break; }
            }
            if (!ok) continue;
            for (int r = 0; r < ROWS-1; ++r) {
                if (col_digit[s][r+1] == 0 && col_digit[t][r] == 0) { ok = false; break; }
            }
            if (ok) arr[idx++] = (int)t;
        }
    }
}

/* ----------------------------------------------------------------------
 *  Main sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();                     /* ensure tables are ready        */
    if (n == 0) return (seq_t)0;     /* undefined behaviour below 1   */

    /* Allocate DP arrays (static to avoid hot‑path construction)     */
    static seq_t dp[MAX_COLS];
    static seq_t ndp[MAX_COLS];

    /* initialise dp to 1 for every valid state                       */
    for (size_t i = 0; i < state_count; ++i) {
        dp[i] = (seq_t)1;
    }

    /* dynamic programming over columns                               */
    for (idx_t col = 2; col <= n; ++col) {
        /* zero ndp */
        for (size_t i = 0; i < state_count; ++i) ndp[i] = (seq_t)0;

        /* transition */
        for (size_t s = 0; s < state_count; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            int *list = succ[s];
            size_t ccnt = succ_cnt[s];
            for (size_t k = 0; k < ccnt; ++k) {
                ndp[list[k]] += val;
            }
        }
        /* swap dp and ndp */
        for (size_t i = 0; i < state_count; ++i) dp[i] = ndp[i];
    }

    /* sum over final states */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < state_count; ++i) result += dp[i];
    return result;
}
