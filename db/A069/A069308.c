/*
 * A069308  Number of 4 X n binary arrays with a path of adjacent 1's from
 *          upper left corner to anywhere in right hand column.
 * https://oeis.org/A069308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069308: number of 4×n binary arrays with a path of adjacent 1's
 *  from the upper‑left corner to any cell in the rightmost column.
 *
 *  The algorithm uses a transfer–matrix / state‑compression dynamic
 *  programming.  For each processed column we keep a state that
 *  records, for each of the four cells in the current frontier,
 *  the component label of that cell (or LAB_UNSET if the cell is 0).
 *  One of those components contains the starting cell (top‑left); its
 *  label is stored separately.  A transition consists of adding a new
 *  column (chosen as a 4‑bit mask) and merging components that become
 *  adjacent vertically within the new column or horizontally with the
 *  previous frontier.  States whose start component disappears are
 *  discarded.  Because at most 4 cells are in a frontier, the number of
 *  distinct states is very small (≤ 60) and a simple linear search
 *  suffices for the state map.
 *
 *  Complexity:  O(n * S * 16) with S the number of states
 *  (≈ 50), well below 2 s for n ≤ 10000.
 *
 *  The function uses only integer arithmetic; all intermediate
 *  values fit in the type seq_t or never overflow (the sequence
 *  grows exponentially but fits in seq_t for the tested values).
 */

#include <stddef.h>
#include <stdint.h>

#define LAB_UNSET 255U
#define MAX_STATES 256U   /* safe upper bound; only a few states are possible */

typedef struct {
    unsigned char lab[4];   /* component id of each cell (LAB_UNSET if 0) */
    unsigned char start;    /* label of the component containing the start cell */
    unsigned char compCnt;  /* number of components in this frontier */
} State;

typedef struct {
    State st;
    seq_t val;              /* number of ways to reach this state */
} Item;

/* find root in union‑find structure */
static int uf_find(int *par, int x)
{
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

/* union of two components in union‑find (by rank) */
static void uf_union(int *par, int *rnk, int a, int b)
{
    a = uf_find(par, a);
    b = uf_find(par, b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) {
        par[a] = b;
    } else if (rnk[a] > rnk[b]) {
        par[b] = a;
    } else {
        par[b] = a;
        rnk[a] += 1;
    }
}

/* Test whether the start component survives after adding a new column
 * with pattern pat.  If it does, new state is stored in *out
 * and function returns 1; otherwise 0. */
static int apply_pattern(const State *old, uint8_t pat, State *out)
{
    const int ROWS = 4;
    unsigned char new_lab[ROWS];
    int id_map[ROWS];          /* provisional id for each new cell, or LAB_UNSET */
    int old_cnt    = old->compCnt;
    int new_cnt    = 0;        /* number of cells in new column that are 1 */

    /* Assign provisional ids to new column cells */
    for (int r = 0; r < ROWS; ++r) {
        if (pat & (1U << r)) {          /* cell is 1 in new column */
            if (old->lab[r] == LAB_UNSET) {
                id_map[r] = old_cnt + new_cnt;
                ++new_cnt;
            } else {
                id_map[r] = old->lab[r];  /* inherits label from old frontier */
            }
        } else {
            id_map[r] = LAB_UNSET;
        }
    }

    int total = old_cnt + new_cnt;
    if (total > 8) return 0;    /* safety guard: will never happen for 4 rows */

    int par[8], rnk[8];
    for (int i = 0; i < total; ++i) {
        par[i] = i;
        rnk[i] = 0;
    }

    /* merge horizontally between old frontier and new column */
    for (int r = 0; r < ROWS; ++r) {
        if ((pat & (1U << r)) && old->lab[r] != LAB_UNSET) {
            uf_union(par, rnk, id_map[r], old->lab[r]);
        }
    }

    /* merge vertically within the new column */
    for (int r = 0; r < ROWS - 1; ++r) {
        if ((pat & (1U << r)) && (pat & (1U << (r + 1)))) {
            uf_union(par, rnk, id_map[r], id_map[r + 1]);
        }
    }

    /* Canonical renumbering of components */
    unsigned char newLabel[8];
    for (int i = 0; i < 8; ++i) newLabel[i] = LAB_UNSET;
    unsigned char newCompCnt = 0;

    for (int r = 0; r < ROWS; ++r) {
        if (pat & (1U << r)) {
            int root = uf_find(par, id_map[r]);
            if (newLabel[root] == LAB_UNSET) {
                newLabel[root] = newCompCnt++;
            }
            new_lab[r] = newLabel[root];
        } else {
            new_lab[r] = LAB_UNSET;
        }
    }

    /* Determine new start component id */
    int startOld = old->start;
    int rootStart = uf_find(par, startOld);
    if (newLabel[rootStart] == LAB_UNSET) {
        return 0;    /* start component has disappeared */
    }

    out->compCnt = newCompCnt;
    out->start   = newLabel[rootStart];
    for (int r = 0; r < ROWS; ++r) out->lab[r] = new_lab[r];
    return 1;
}

/* Linear search for a state in an array of Items */
static int find_state(const Item *arr, size_t cnt, const State *st)
{
    for (size_t i = 0; i < cnt; ++i) {
        if (st->compCnt != arr[i].st.compCnt) continue;
        if (st->start   != arr[i].st.start)   continue;
        int eq = 1;
        for (int r = 0; r < 4; ++r) {
            if (st->lab[r] != arr[i].st.lab[r]) { eq = 0; break; }
        }
        if (eq) return (int)i;
    }
    return -1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    Item cur[MAX_STATES], nxt[MAX_STATES];
    size_t curCnt = 0, nxtCnt;

    /* ---- Initial states after processing the first column ---- */
    for (uint8_t pat = 0; pat < 16; ++pat) {
        if (!(pat & 1U)) continue;          /* top-left cell must be 1 */

        unsigned char lab[4];
        unsigned char cnt = 1;               /* top cell forms component 0 */
        lab[0] = 0;
        for (int r = 1; r < 4; ++r) {
            if (pat & (1U << r)) {
                if (pat & (1U << (r - 1))) {
                    lab[r] = lab[r - 1];
                } else {
                    lab[r] = cnt++;
                }
            } else {
                lab[r] = LAB_UNSET;
            }
        }

        State s;
        s.compCnt = cnt;
        s.start   = lab[0];                 /* start is component 0 */
        for (int r = 0; r < 4; ++r) s.lab[r] = lab[r];

        cur[curCnt].st   = s;
        cur[curCnt].val  = (seq_t)1;
        ++curCnt;
    }

    /* ---- Transition through the remaining columns ---- */
    for (idx_t col = 2; col <= n; ++col) {
        nxtCnt = 0;
        for (size_t i = 0; i < curCnt; ++i) {
            seq_t curVal = cur[i].val;
            for (uint8_t pat = 0; pat < 16; ++pat) {
                State ns;
                if (!apply_pattern(&cur[i].st, pat, &ns)) continue;
                int idx = find_state(nxt, nxtCnt, &ns);
                if (idx >= 0) {
                    nxt[idx].val += curVal;
                } else {
                    nxt[nxtCnt].st   = ns;
                    nxt[nxtCnt].val  = curVal;
                    ++nxtCnt;
                }
            }
        }
        curCnt = nxtCnt;
        for (size_t i = 0; i < curCnt; ++i) cur[i] = nxt[i];
    }

    /* ---- Sum all surviving states (start component is present) ---- */
    seq_t result = 0;
    for (size_t i = 0; i < curCnt; ++i) result += cur[i].val;
    return result;
}
