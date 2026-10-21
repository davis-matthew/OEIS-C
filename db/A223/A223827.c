/*
 * A223827  Number of n X 4 0..2 arrays with rows and columns unimodal.
 * https://oeis.org/A223827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries in {0,1,2} such that
 *            every row and every column is unimodal (first non‑decreasing,
 *            then non‑increasing). The function evaluates this combinatorial
 *            count. The DP builds the arrays row by row, keeping for each
 *            column the last value and whether a decreasing s...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 41, u128 n <= 210,
 *            big any n
 * Helpers:   is_row_unimodal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_row_unimodal.h"

/* State encoding: for each column (0..3) we store a 3‐bit code
   value*2 + dec, where dec is 0 before the decreasing part
   starts and 1 afterwards.  The overall state is
   sum_{j=0..3} code_j * 6^j.  Hence there are 6^4 = 1296 states. */
#define MAX_STATE 1296
static const int POW6[4] = {1, 6, 36, 216};

/* All unimodal rows of length 4 over {0,1,2}.  There are 46 of them. */
enum { MAX_ROWS = 81 };
typedef struct { uint8_t v[4]; } row_t;
static row_t unimodal_rows[MAX_ROWS];
static int num_unimodal_rows = 0;

/* Transition tables: for each state the list of possible next states */
#define MAX_TRANS 81
static int next_states[MAX_STATE][MAX_TRANS];
static int next_counts[MAX_STATE];

static bool init_done = false;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Precompute all unimodal rows and the transition tables. */
static void init(void) {
    /* Enumerate all 3^4 rows and keep the unimodal ones. */
    for (int a = 0; a <= 2; ++a)
    for (int b = 0; b <= 2; ++b)
    for (int c = 0; c <= 2; ++c)
    for (int d = 0; d <= 2; ++d) {
        uint8_t r[4] = { (uint8_t)a, (uint8_t)b, (uint8_t)c, (uint8_t)d };
        if (is_row_unimodal(r)) {
            unimodal_rows[num_unimodal_rows].v[0] = r[0];
            unimodal_rows[num_unimodal_rows].v[1] = r[1];
            unimodal_rows[num_unimodal_rows].v[2] = r[2];
            unimodal_rows[num_unimodal_rows].v[3] = r[3];
            ++num_unimodal_rows;
        }
    }

    /* Build the transition table for each state. */
    for (int s = 0; s < MAX_STATE; ++s) {
        int rem = s;
        int prev_val[4];
        int prev_dec[4];
        for (int j = 0; j < 4; ++j) {
            int code = rem % 6;
            rem /= 6;
            prev_val[j] = code / 2;
            prev_dec[j] = code % 2;
        }
        int cnt = 0;
        for (int ridx = 0; ridx < num_unimodal_rows; ++ridx) {
            const uint8_t *rowv = unimodal_rows[ridx].v;
            bool ok = true;
            int new_state = 0;
            for (int j = 3; j >= 0; --j) {
                int new_val = rowv[j];
                int new_dec;
                if (prev_dec[j] == 0) {
                    if (new_val < prev_val[j])
                        new_dec = 1;
                    else
                        new_dec = 0;
                } else {        /* already decreasing */
                    if (new_val > prev_val[j]) { ok = false; break; }
                    new_dec = 1;
                }
                int code = new_val * 2 + new_dec;
                new_state += code * POW6[j];
            }
            if (ok) {
                next_states[s][cnt++] = new_state;
            }
        }
        next_counts[s] = cnt;
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main function required by the build */
seq_t a(idx_t n) {
    if (n == 0)   /* offset is 1, but keep it defined */
        return (seq_t)0;
    if (!init_done)
        init();

    seq_t dp[MAX_STATE] = {0};
    seq_t newdp[MAX_STATE];

    /* Initialise with the first row */
    for (int ridx = 0; ridx < num_unimodal_rows; ++ridx) {
        int s = 0;                          /* all dec=0 initially */
        for (int j = 3; j >= 0; --j) {
            int val = unimodal_rows[ridx].v[j];
            int code = val * 2;              /* dec = 0 */
            s += code * POW6[j];
        }
        dp[s] += (seq_t)1;
    }
    if (n == 1) {                           /* sum over all states */
        seq_t sum = 0;
        for (int s = 0; s < MAX_STATE; ++s)
            sum += dp[s];
        return sum;
    }

    /* Add subsequent rows */
    for (idx_t step = 1; step < n; ++step) {
        for (int s = 0; s < MAX_STATE; ++s)
            newdp[s] = 0;
        for (int s = 0; s < MAX_STATE; ++s) {
            if (dp[s] == (seq_t)0) continue;
            for (int k = 0; k < next_counts[s]; ++k) {
                int s2 = next_states[s][k];
                newdp[s2] += dp[s];
            }
        }
        for (int s = 0; s < MAX_STATE; ++s)
            dp[s] = newdp[s];
    }

    /* Total count */
    seq_t total = 0;
    for (int s = 0; s < MAX_STATE; ++s)
        total += dp[s];
    return total;
}
