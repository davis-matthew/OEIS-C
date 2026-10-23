/*
 * A180753  Half the number of nX4 binary arrays with each element equal to at
 *          least two neighbors.
 * https://oeis.org/A180753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A180753:
 *   a(n) = number of n×4 binary arrays in which each cell has at least
 *   two equal neighbors, divided by 2.
 *
 *   The value is obtained by a transfer‑matrix dynamic program over the
 *   4 columns.  Each state consists of the bits of the previous row
 *   (4 bits) and a 4‑bit mask marking cells that presently have only
 *   one equal neighbor (so they must match the next row).  The table
 *   of transitions between two consecutive rows is pre‑computed once.
 *
 *   This implementation works for all builds of seq_t: 8/16/32/64-bit
 *   unsigned integers or arbitrary‑precision integer classes, and is
 *   valid C11 and C++17.  It does not use memset on arrays of seq_t
 *   to avoid undefined behaviour for class types. */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool valid;
    idx_t pending;          /* 4‑bit mask of cells that are pending */
} pending_info;

seq_t a(idx_t n) {
    /* dp[state] holds number of ways to build rows up to the current row
     * where 'state' encodes (prev_row << 4) | pending_mask. */
    seq_t dp[256], ndp[256];

    /* initialise dp and ndp to zero (no memset to keep C/C++ compatibility) */
    for (idx_t i = 0; i < 256; ++i) {
        dp[i] = (seq_t)0;
        ndp[i] = (seq_t)0;
    }

    /* Transition table between adjacent rows. */
    static bool initialized = false;
    static pending_info table[16][16];
    if (!initialized) {
        initialized = true;
        for (idx_t up = 0; up < 16; ++up) {
            for (idx_t pat = 0; pat < 16; ++pat) {
                idx_t pending = 0;
                bool valid = true;
                for (idx_t j = 0; j < 4; ++j) {
                    idx_t v = (pat >> j) & 1;
                    idx_t cnt = 0;
                    /* horizontal neighbours */
                    if (j > 0 && v == ((pat >> (j - 1)) & 1)) ++cnt;
                    if (j < 3 && v == ((pat >> (j + 1)) & 1)) ++cnt;
                    /* vertical neighbour from previous row */
                    if (v == ((up >> j) & 1)) ++cnt;
                    /* a cell must have at least two equal neighbours */
                    if (cnt == 0) { valid = false; break; }
                    if (cnt == 1) pending |= (idx_t)1 << j;
                }
                table[up][pat].valid = valid;
                table[up][pat].pending = pending;
            }
        }
    }

    /* ---- initialise first row ---- */
    for (idx_t pat = 0; pat < 16; ++pat) {
        idx_t pending = 0;
        bool valid = true;
        for (idx_t j = 0; j < 4; ++j) {
            idx_t v = (pat >> j) & 1;
            idx_t cnt = 0;
            if (j > 0 && v == ((pat >> (j - 1)) & 1)) ++cnt;
            if (j < 3 && v == ((pat >> (j + 1)) & 1)) ++cnt;
            /* no up neighbour for the first row */
            if (cnt == 0) { valid = false; break; }
            if (cnt == 1) pending |= (idx_t)1 << j;
        }
        if (!valid) continue;
        dp[(pat << 4) | pending] = (seq_t)1;
    }

    /* ---- process remaining rows ---- */
    for (idx_t row = 1; row < n; ++row) {
        /* reset ndp */
        for (idx_t i = 0; i < 256; ++i) ndp[i] = (seq_t)0;

        for (idx_t state = 0; state < 256; ++state) {
            seq_t ways = dp[state];
            if (ways == (seq_t)0) continue;
            idx_t prev_pat    = state >> 4;
            idx_t prev_pending = state & 0xF;

            for (idx_t pat = 0; pat < 16; ++pat) {
                /* pending cells of the previous row must match the new row */
                if (prev_pending & (prev_pat ^ pat)) continue;

                const pending_info *pi = &table[prev_pat][pat];
                if (!pi->valid) continue;

                idx_t new_state = (pat << 4) | pi->pending;
                ndp[new_state] += ways;
            }
        }
        /* swap dp and ndp */
        for (idx_t i = 0; i < 256; ++i) dp[i] = ndp[i];
    }

    /* ---- sum all states with no pending cells (completed arrays) ---- */
    seq_t total = (seq_t)0;
    for (idx_t state = 0; state < 256; ++state) {
        if ((state & 0xF) == 0) total += dp[state];
    }

    /* The sequence value is half the number of arrays. */
    return total / (seq_t)2;
}
