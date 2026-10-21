/*
 * A240652  Number of nX4 0..1 arrays with no element equal to exactly one
 *          horizontal or vertical neighbor, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   bit_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A240652
 *  Formula implemented with column‑by‑column dynamic programming.
 *  State: 12 bits -> 4 bits for the column pattern (value of each of the
 *  four rows 0..3) and 8 bits for, for each row, the number of equal
 *  neighbours already known before the next column is processed.
 *  Columns are processed left to right; at each step the neighbour on the
 *  right (i.e. the next column) is still unknown, therefore the state
 *  keeps the count of equal neighbours coming from the left and from the
 *  vertical neighbours inside the column.  For a cell with current
 *  count k and a new neighbour value q, the total number of equal
 *  neighbours becomes k + (current == q).  The forbidden situation is
 *  exactly one equal neighbour, i.e. k + (current == q) == 1.
 *  The first cell (top‑left) may not be 1, otherwise a second distinct
 *  value would be introduced before the value 0.  This is enforced by
 *  only allowing initial column patterns with their top‑row bit equal
 *  to 0. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/bit_at.h"

#define ROWS          4
#define PAT_BITS      ROWS
#define K_BITS_PER_R  2
#define STATE_BITS    (PAT_BITS + ROWS*K_BITS_PER_R)   // 12 bits
#define STATE_COUNT   (1U << STATE_BITS)              // 4096
#define COL_PAT_TBL   16                               // 4‑bit patterns

static int16_t trans[STATE_COUNT][COL_PAT_TBL];
static int8_t  init_done = 0;

/* Initialise the transition table once.  The table entry is the index
 * of the next state after adding column 'q' to state 's'; -1 denotes
 * an invalid transition. */
static void init_transitions(void)
{
    if (init_done)
        return;
    for (int s = 0; s < STATE_COUNT; ++s) {
        int pat = s >> 8;          /* current column pattern (4 bits)   */
        int kcode = s & 0xFF;      /* current counts for the 4 rows     */
        for (int q = 0; q < COL_PAT_TBL; ++q) {   /* next column pattern      */
            int valid = 1;
            /* Check if the next column satisfies the constraint for
             * the cells of the current column. */
            for (int r = 0; r < ROWS; ++r) {
                int cur_k = (kcode >> (2*r)) & 3;
                int same = (bit_at(pat, r) == bit_at(q, r)) ? 1 : 0;
                if (cur_k + same == 1) { /* forbidden              */
                    valid = 0;
                    break;
                }
            }
            if (!valid) {
                trans[s][q] = -1;
                continue;
            }
            /* Compute k' for the next column: number of equal neighbours
             * already known (left neighbour + vertical ones).          */
            int new_kcode = 0;
            for (int r = 0; r < ROWS; ++r) {
                int vsame = 0;
                if (r > 0)   vsame += (bit_at(q, r) == bit_at(q, r-1)) ? 1 : 0;
                if (r < ROWS-1) vsame += (bit_at(q, r) == bit_at(q, r+1)) ? 1 : 0;
                int lsame = (bit_at(pat, r) == bit_at(q, r)) ? 1 : 0;
                int total = vsame + lsame;   /* 0..3                          */
                new_kcode |= (total << (2*r));
            }
            int nstate = (q << 8) | new_kcode;
            trans[s][q] = (int16_t)nstate;
        }
    }
    init_done = 1;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Special case: n <= 0 is undefined; return 0. */
    if (n == 0)
        return (seq_t)0;

    init_transitions();

    /* DP arrays: dp and ndp store counts for the current and next column. */
    static seq_t dp[STATE_COUNT];
    static seq_t ndp[STATE_COUNT];

    /* Clear dp.  Since dp is static it may contain values from a previous
     * call; set all entries to 0. */
    for (idx_t i = 0; i < STATE_COUNT; ++i)
        dp[i] = 0;

    /* Initialise dp with the first column patterns (top‑row bit must be 0). */
    for (int pat = 0; pat < COL_PAT_TBL; ++pat) {
        if ((pat & 1) != 0)      /* top‑row bit must be 0            */
            continue;
        /* Compute vertical equal counts (only up/down, no left). */
        int kcode = 0;
        for (int r = 0; r < ROWS; ++r) {
            int vs = 0;
            if (r > 0)   vs += (bit_at(pat, r) == bit_at(pat, r-1)) ? 1 : 0;
            if (r < ROWS-1) vs += (bit_at(pat, r) == bit_at(pat, r+1)) ? 1 : 0;
            /* vs is 0..2; store in 2 bits. */
            kcode |= (vs << (2*r));
        }
        int state = (pat << 8) | kcode;
        dp[state] = 1;
    }

    /* DP over the remaining n-1 columns. */
    for (idx_t col = 1; col < n; ++col) {
        /* Zero ndp. */
        for (idx_t i = 0; i < STATE_COUNT; ++i)
            ndp[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == 0)
                continue;
            seq_t cur_val = dp[s];
            for (int q = 0; q < COL_PAT_TBL; ++q) {
                int16_t ns = trans[s][q];
                if (ns >= 0)
                    ndp[ns] += cur_val;
            }
        }
        /* Swap dp and ndp. */
        for (idx_t i = 0; i < STATE_COUNT; ++i)
            dp[i] = ndp[i];
    }

    /* Sum over final states: no cell may have exactly one equal neighbour.
     * That means for each row r: k[r] != 1. */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (dp[s] == 0)
            continue;
        int kcode = s & 0xFF;
        int ok = 1;
        for (int r = 0; r < ROWS; ++r) {
            if (((kcode >> (2*r)) & 3) == 1) {
                ok = 0;
                break;
            }
        }
        if (ok)
            result += dp[s];
    }
    return result;
}
