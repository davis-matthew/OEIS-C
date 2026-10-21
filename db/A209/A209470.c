/*
 * A209470  Number of nX2 0..2 arrays with every element value z a city block
 *          distance of exactly z from another element value z.
 * https://oeis.org/A209470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A209470: Number of n×2 arrays with entries 0,1,2 such that
 * each element of value z has another element of the same value z at
 * Manhattan distance exactly z.  The function uses dynamic programming
 * over the last two columns.  Each state stores the values and a
 * satisfaction flag for the 4 cells of the two most recent columns.
 * After adding a new column, all cells in the column that is now two
 * columns behind must be satisfied; otherwise the transition is
 * forbidden.  States where the two most recent columns are fully
 * satisfied are added at the end.
 */

#include <stddef.h>

#define MAX_STATE 1296   /* 6⁴ possible combinations of (value, sat) for 4 cells */

typedef struct {
    unsigned val : 2;   /* value 0..2 */
    unsigned sat : 1;   /* 0 = not yet satisfied, 1 = satisfied */
} cell_t;

/* Encode 4 cells into a state id in 0..1295
 * The encoding is base-6: cell[i] = value*2 + sat, with value in [0,2] and sat in {0,1}.
 */
static int encode_state(const cell_t c[4]) {
    int code = 0;
    int mult = 1;
    for (int i = 0; i < 4; ++i) {
        code += mult * ((c[i].val << 1) | c[i].sat);
        mult *= 6;
    }
    return code;
}

/* Decode a state id into 4 cells */
static void decode_state(int id, cell_t c[4]) {
    int tmp = id;
    for (int i = 0; i < 4; ++i) {
        int val_sat = tmp % 6;
        tmp /= 6;
        c[i].val = val_sat >> 1;   /* value = val_sat / 2 */
        c[i].sat = val_sat & 1;    /* sat   = val_sat % 2 */
    }
}

/* Pre‑computed Manhattan distances between the 6 cells in a 3‑column window
 * Order of cells:
 *   0: column -2, top
 *   1: column -2, bottom
 *   2: column -1, top
 *   3: column -1, bottom
 *   4: column 0,  top (new column)
 *   5: column 0,  bottom (new column)
 */
static const int dist[6][6] = {
    {0,1,1,2,2,3},
    {1,0,2,1,3,2},
    {1,2,0,1,1,2},
    {2,1,1,0,2,1},
    {2,3,1,2,0,1},
    {3,2,2,1,1,0}
};

/* Build the transition table once.
 * trans[s][t] = next_state or -1 if the transition is invalid.
 * There are 1296 states and 9 possible column assignments (3 top × 3 bottom).
 */
static void build_transitions(int trans[1296][9]) {
    for (int s = 0; s < MAX_STATE; ++s) {
        cell_t old[4];
        decode_state(s, old);
        for (int t = 0; t < 9; ++t) {
            int top = t / 3;   /* 0..2 */
            int bot = t % 3;   /* 0..2 */

            /* Assemble the 6 cells in the window */
            cell_t all[6];
            for (int i = 0; i < 4; ++i) {
                all[i] = old[i];
            }
            all[4].val = top;
            all[4].sat = (top == 0) ? 1 : 0;
            all[5].val = bot;
            all[5].sat = (bot == 0) ? 1 : 0;

            /* Check all pairs within the window and set satisfaction flags */
            for (int i = 0; i < 6; ++i) {
                for (int j = i + 1; j < 6; ++j) {
                    if (all[i].val == 0 || all[j].val == 0) continue;
                    if (all[i].val == all[j].val && dist[i][j] == all[i].val) {
                        all[i].sat = 1;
                        all[j].sat = 1;
                    }
                }
            }

            /* After all checks, column -2 (cells 0 and 1) must be satisfied */
            if (all[0].sat == 0 || all[1].sat == 0) {
                trans[s][t] = -1;
                continue;
            }

            /* The new state consists of columns -1 and 0, i.e. cells 2,3,4,5 */
            cell_t nxt[4];
            for (int i = 0; i < 4; ++i) {
                nxt[i] = all[i + 2];
            }
            trans[s][t] = encode_state(nxt);
        }
    }
}

/* ----------------------------------------------------------------------
 * Main sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Lazy static initialization of the transition table */
    static int trans[MAX_STATE][9];
    static int initialized = 0;
    if (!initialized) {
        build_transitions(trans);
        initialized = 1;
    }

    /* DP arrays */
    static seq_t dp_cur[MAX_STATE], dp_next[MAX_STATE];

    /* Initial state: two dummy columns of zeros, all satisfied */
    cell_t init_cells[4] = { {0,1}, {0,1}, {0,1}, {0,1} };
    int init_id = encode_state(init_cells);
    for (int i = 0; i < MAX_STATE; ++i) dp_cur[i] = 0;
    dp_cur[init_id] = 1;

    /* Process each column */
    for (idx_t col = 1; col <= n; ++col) {
        for (int i = 0; i < MAX_STATE; ++i) dp_next[i] = 0;
        for (int s = 0; s < MAX_STATE; ++s) {
            seq_t ways = dp_cur[s];
            if (!ways) continue;
            for (int t = 0; t < 9; ++t) {
                int ns = trans[s][t];
                if (ns < 0) continue;
                dp_next[ns] += ways;
            }
        }
        /* copy next to current */
        for (int i = 0; i < MAX_STATE; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over states where both columns are fully satisfied */
    seq_t result = 0;
    for (int s = 0; s < MAX_STATE; ++s) {
        if (!dp_cur[s]) continue;
        int id = s;
        int ok = 1;
        for (int k = 0; k < 4; ++k) {
            int cell = id % 6;
            id /= 6;
            if ((cell & 1) == 0) { /* sat == 0 */
                ok = 0;
                break;
            }
        }
        if (ok) result += dp_cur[s];
    }
    return result;
}
