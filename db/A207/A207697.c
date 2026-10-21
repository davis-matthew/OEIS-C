/*
 * A207697  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   # of 6×n binary arrays avoiding horizontal triples 000,010 and
 *            vertical triples 001,111. Computation: transfer‑matrix DP over
 *            allowed column states. Initial values: a(1)=18, a(2)=324
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int init = 0;                       /* tables constructed once */
static int allowed_vert[64];               /* vertical‑valid columns */
static int vert[64];                       /* list of allowed column indices */
static int column_to_idx[64];              /* map column→index in vert array */

#define MAX_COLUMNS 64                     /* 2^6 possible columns */
#define MAX_MATRIX (MAX_COLUMNS*MAX_COLUMNS) /* max number of pair states (64×64) */

/* pair_id[u][v] gives state index for the pair (u,v) where both are vertical‑valid */
/* using 64×64 array (first 64 entries are u, next 64 are v) */
static int pair_id[64][64];

/* adjacency table for transitions:
   next_state[s*MAX_COLUMNS + w] = new state index for pair (second_of_s, w) or -1 if forbidden */
static int next_state[MAX_MATRIX * MAX_COLUMNS];

/* DP state arrays (held statically to avoid re‑allocation) */
static seq_t dp_cur[MAX_MATRIX];
static seq_t dp_next[MAX_MATRIX];

seq_t a(idx_t n)
{
    static size_t M = 0;        /* number of vertical‑valid columns          */
    static size_t S = 0;        /* number of states M*M                      */

    /* ---------- build tables once ------------------------------------ */
    if (!init) {
        init = 1;

        /* 1. vertical validity of each 6‑bit column */
        for (int i = 0; i < 64; ++i)
            column_to_idx[i] = -1;

        int m = 0;
        for (int col = 0; col < 64; ++col) {
            int ok = 1;
            for (int r = 0; r < 4; ++r) {          /* check the 4 windows of 3 rows */
                int w = (col >> r) & 7;            /* 3 bits: rows r, r+1, r+2  */
                if (w == 0b001 || w == 0b111) {    /* forbidden vertically    */
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                allowed_vert[col] = 1;
                vert[m] = col;
                column_to_idx[col] = m;
                ++m;
            } else {
                allowed_vert[col] = 0;
            }
        }
        M = m;                                  /* number of allowed columns */
        S = M * M;                              /* number of pair states   */

        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                pair_id[i][j] = -1;

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                pair_id[vert[i]][vert[j]] = i * M + j;

        /* 2. build transition table */
        for (size_t idx = 0; idx < S * MAX_COLUMNS; ++idx)
            next_state[idx] = -1;

        for (int i = 0; i < M; ++i) {
            int u = vert[i];
            for (int j = 0; j < M; ++j) {
                int v = vert[j];
                size_t s = i * M + j;            /* state index for (u,v)     */
                int base = s * MAX_COLUMNS;
                for (int k = 0; k < M; ++k) {
                    int w = vert[k];
                    int ok = 1;
                    for (int r = 0; r < 6; ++r) { /* check all 6 rows for triple */
                        int b1 = (u >> r) & 1;
                        int b2 = (v >> r) & 1;
                        int b3 = (w >> r) & 1;
                        int t  = (b1 << 2) | (b2 << 1) | b3;
                        if (t == 0 || t == 2) {     /* 000 or 010 forbidden      */
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        int ns = pair_id[v][w];
                        if (ns >= 0)
                            next_state[base + k] = ns;
                    }
                }
            }
        }
    }

    /* ---------- special cases ---------------------------------------- */
    if (n == 1) {
        return (seq_t)M;
    }
    if (n == 2) {
        seq_t t = (seq_t)M;
        return t * t;
    }

    /* ---------- dynamic programming------------------------------------- */
    seq_t *cur  = dp_cur;
    seq_t *next = dp_next;

    /* initialize counts for sequences of length 2: each pair is allowed */
    for (size_t s = 0; s < S; ++s)
        cur[s] = 1;

    for (idx_t col = 3; col <= n; ++col) {
        /* zero next array */
        for (size_t s = 0; s < S; ++s)
            next[s] = 0;

        for (size_t s = 0; s < S; ++s) {
            seq_t cnt = cur[s];
            if (!cnt) continue;
            int base = (int)(s * MAX_COLUMNS);
            for (int k = 0; k < (int)M; ++k) {
                int ns = next_state[base + k];
                if (ns >= 0)
                    next[ns] += cnt;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all states to get total number of matrices of width n */
    seq_t total = 0;
    for (size_t s = 0; s < S; ++s)
        total += cur[s];

    return total;
}
