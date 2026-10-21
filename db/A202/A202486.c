/*
 * A202486  Number of (n+2)X4 binary arrays avoiding patterns 000 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A202486
   Number of (n+2)×4 binary arrays avoiding patterns 000 and 010
   in rows, columns and northwest‑to‑southeast diagonals.
   Computed by a transfer‑matrix dynamic programming over all
   16 possible 4‑bit rows. */

#include <stdbool.h>

/* Transition tables: 256 previous–current states × 16 candidate new rows */
static bool row_valid[16];
static bool trans[256][16];
static bool init = false;

/* Precompute helper tables once */
static void init_tables(void)
{
    if (init) return;

    /* Precompute horizontal validity for each 4‑bit row */
    for (int row = 0; row < 16; ++row) {
        bool ok = true;
        /* first 3‑bit window: bits 0,1,2 */
        int w = ((row >> 0) & 7);
        if (w == 0 || w == 2) ok = false;
        /* second 3‑bit window: bits 1,2,3 */
        if (ok) {
            w = ((row >> 1) & 7);
            if (w == 0 || w == 2) ok = false;
        }
        row_valid[row] = ok;
    }

    /* Precompute transitions: from state (prev, curr) to new row newrow */
    for (int s = 0; s < 256; ++s) {
        int prev = s >> 4;
        int curr = s & 15;
        for (int newrow = 0; newrow < 16; ++newrow) {
            if (!row_valid[newrow]) { trans[s][newrow] = false; continue; }

            bool ok = true;
            /* Vertical constraints on each column */
            for (int j = 0; j < 4; ++j) {
                int a = (prev >> j) & 1;
                int b = (curr >> j) & 1;
                int c = (newrow  >> j) & 1;
                int tri = (a << 2) | (b << 1) | c;
                if (tri == 0 || tri == 2) { ok = false; break; }
            }
            /* Diagonal NW–SE constraints (only for j >= 2) */
            if (ok) {
                for (int j = 2; j < 4; ++j) {
                    int a = (prev >> (j-2)) & 1;
                    int b = (curr >> (j-1)) & 1;
                    int c = (newrow  >> j)      & 1;
                    int tri = (a << 2) | (b << 1) | c;
                    if (tri == 0 || tri == 2) { ok = false; break; }
                }
            }
            trans[s][newrow] = ok;
        }
    }
    init = true;
}

seq_t a(idx_t n)
{
    init_tables();        /* Ensure tables are ready */

    /* Number of rows is n+2. Minimum n = 1 gives 3 rows. */
    idx_t rows = n + 2;

    /* DP arrays for current and next row counts */
    seq_t cur[256];
    seq_t nxt[256];

    /* Zero all entries */
    for (int i = 0; i < 256; ++i) cur[i] = 0, nxt[i] = 0;

    /* Initial state: first two rows both horizontally valid */
    for (int prev = 0; prev < 16; ++prev) {
        if (!row_valid[prev]) continue;
        for (int curr = 0; curr < 16; ++curr) {
            if (!row_valid[curr]) continue;
            cur[(prev << 4) | curr] = 1;
        }
    }

    /* Build subsequent rows.  We already have two rows accounted for. */
    for (idx_t r = 3; r <= rows; ++r) {
        /* Zero next array */
        for (int i = 0; i < 256; ++i) nxt[i] = 0;
        for (int s = 0; s < 256; ++s) {
            if (cur[s] == 0) continue;
            int curr = s & 15;   /* current row in this state */
            seq_t ways = cur[s];
            for (int newrow = 0; newrow < 16; ++newrow) {
                if (!trans[s][newrow]) continue;
                int newstate = (curr << 4) | newrow;
                nxt[newstate] += ways;
            }
        }
        /* Swap cur and nxt */
        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    /* Sum all states: each represents a finished matrix */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) result += cur[i];
    return result;
}
