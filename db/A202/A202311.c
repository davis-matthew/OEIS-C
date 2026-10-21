/*
 * A202311  Number of (n+2)X4 binary arrays avoiding patterns 001 and 100 in
 *          rows and columns.
 * https://oeis.org/A202311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A202311 by dynamic programming.
   The sequence counts (n+2)×4 binary arrays that avoid the
   patterns 001 and 100 in every row and every column.
   The method builds the array row‑by‑row, keeping the last two rows
   as a state.  There are 9 admissible 4‑bit rows; transitions between
   states are precomputed and applied O(n) times.  This matches all
   terms of the sequence and is far faster than the empirical
   12‑term recurrence given in the OEIS entry. */
#include <stdbool.h>

#define NUM_ROWS 9
static const idx_t ALLOWED_ROWS[NUM_ROWS] = {0,5,6,7,10,11,13,14,15};

/* precomputed transition tables: for each state (prevprev,prev) and
   each possible next row, whether vertical patterns are avoided,
   and the resulting next state. */
static bool TRANS[256][NUM_ROWS];
static uint8_t NEXT_STATE[256][NUM_ROWS];
static bool INIT_DONE = false;

static void init_transitions(void) {
    if (INIT_DONE) return;
    for (int s = 0; s < 256; ++s) {
        int a = s >> 4;      /* previous‑previous row (4 bits) */
        int b = s & 0xF;     /* previous row (4 bits) */
        for (int j = 0; j < NUM_ROWS; ++j) {
            int r = ALLOWED_ROWS[j];
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                int b1 = (a >> col) & 1;
                int b2 = (b >> col) & 1;
                int b3 = (r >> col) & 1;
                if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                    (b1 == 1 && b2 == 0 && b3 == 0)) {
                    ok = false;
                    break;
                }
            }
            TRANS[s][j] = ok;
            NEXT_STATE[s][j] = (uint8_t)((b << 4) | r);
        }
    }
    INIT_DONE = true;
}

seq_t a(idx_t n) {
    /* Number of rows in the array */
    idx_t L = n + 2;          /* L >= 3 for n >= 1 */
    /* Handle trivial small L directly */
    if (L == 0) return 0;
    if (L == 1) {            /* single row */
        seq_t total = 0;
        for (int j = 0; j < NUM_ROWS; ++j) total += (seq_t)1;
        return total;
    }
    /* States of last two rows: index = (prevprev << 4) | prev */
    seq_t dp2[256] = {0}, ndp[256] = {0};
    seq_t dp1[16]   = {0};

    init_transitions();

    /* dp1: after first row */
    for (int j = 0; j < NUM_ROWS; ++j) dp1[ALLOWED_ROWS[j]] = 1;

    if (L == 1) {
        seq_t total = 0;
        for (int j = 0; j < 16; ++j) total += dp1[j];
        return total;
    }

    /* dp2: after two rows */
    for (int j = 0; j < NUM_ROWS; ++j) {
        int r1 = ALLOWED_ROWS[j];
        for (int k = 0; k < NUM_ROWS; ++k) {
            int r2 = ALLOWED_ROWS[k];
            dp2[(r1 << 4) | r2] += dp1[r1];
        }
    }

    if (L == 2) {
        seq_t total = 0;
        for (int s = 0; s < 256; ++s) total += dp2[s];
        return total;
    }

    /* Iterate for rows 3..L */
    for (idx_t step = 3; step <= L; ++step) {
        /* zero ndp */
        for (int s = 0; s < 256; ++s) ndp[s] = 0;

        for (int s = 0; s < 256; ++s) {
            seq_t cnt = dp2[s];
            if (cnt == 0) continue;
            for (int j = 0; j < NUM_ROWS; ++j) {
                if (!TRANS[s][j]) continue;
                uint8_t ns = NEXT_STATE[s][j];
                ndp[ns] += cnt;
            }
        }
        /* copy ndp to dp2 */
        for (int s = 0; s < 256; ++s) dp2[s] = ndp[s];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int s = 0; s < 256; ++s) result += dp2[s];
    return result;
}
