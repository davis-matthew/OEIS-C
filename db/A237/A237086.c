/*
 * A237086  Number of (n+1)X(4+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Count (n+1)×5 arrays over {0,1,2,3} whose
 * 2×2 subblocks have the same value
 *    max + upper median + lower median – min.
 * The value is a constant K for the whole matrix.  The code counts such
 * arrays for each possible K (0–9) and sums the results.  It uses a
 * pre‑computed transition graph between rows and dynamic programming. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROWS 1024
#define MAX_K    10   /* possible values of the expression: 0..9 */
#define MAX_COL  5

/* --- pre‑computed data ------------------------------------------- */

/* row representation: each row is 5 digits in base 4 */
static unsigned char row_vals[MAX_ROWS][MAX_COL];

/* expression for a 2×2 block, key = (a<<6)|(b<<4)|(c<<2)|d  (a,b,c,d∈0..3) */
static unsigned char expr_tbl[256];

/* adjacency lists per K: arrays of (from,to) indices */
static unsigned short **trans_from;     /* [K][edge] */
static unsigned short **trans_to;       /* [K][edge] */
static size_t *trans_cnt;               /* [K] number of edges */

/* temporary per‑row counter used during construction */
static unsigned short **perrow_cnt;

/* initialization flag */
static bool init_done = false;

/* ---------------------------------------------------------------- */

static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    /* --- build row values ------------------------------------- */
    for (int id = 0; id < MAX_ROWS; ++id) {
        int x = id;
        for (int pos = MAX_COL-1; pos >= 0; --pos) {
            row_vals[id][pos] = (unsigned char)(x & 3);
            x >>= 2;
        }
    }

    /* --- build expression table -------------------------------- */
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d) {
                    int v[4] = {a, b, c, d};
                    /* simple four‑element sort */
                    for (int i = 0; i < 3; ++i)
                        for (int j = i+1; j < 4; ++j)
                            if (v[i] > v[j]) {
                                int t = v[i];
                                v[i] = v[j];
                                v[j] = t;
                            }
                    unsigned char e = (unsigned char)(v[3] + v[2] + v[1] - v[0]);
                    expr_tbl[(a << 6) | (b << 4) | (c << 2) | d] = e;
                }

    /* --- allocate per‑row counters -------------------------------- */
    perrow_cnt = (unsigned short **)malloc(MAX_K * sizeof(unsigned short *));
    for (int k = 0; k < MAX_K; ++k)
        perrow_cnt[k] = (unsigned short *)calloc(MAX_ROWS, sizeof(unsigned short));

    /* --- first pass: count edges per K -------------------------------- */
    trans_cnt = (size_t *)calloc(MAX_K, sizeof(size_t));
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            int key0 = (row_vals[i][0] << 6) | (row_vals[i][1] << 4)
                     | (row_vals[j][0] << 2) | (row_vals[j][1]);
            int e0   = expr_tbl[key0];
            int key1 = (row_vals[i][1] << 6) | (row_vals[i][2] << 4)
                     | (row_vals[j][1] << 2) | (row_vals[j][2]);
            if (expr_tbl[key1] != e0) continue;
            int key2 = (row_vals[i][2] << 6) | (row_vals[i][3] << 4)
                     | (row_vals[j][2] << 2) | (row_vals[j][3]);
            if (expr_tbl[key2] != e0) continue;
            int key3 = (row_vals[i][3] << 6) | (row_vals[i][4] << 4)
                     | (row_vals[j][3] << 2) | (row_vals[j][4]);
            if (expr_tbl[key3] != e0) continue;

            unsigned char k = (unsigned char)e0;
            ++perrow_cnt[k][i];
            ++trans_cnt[k];
        }
    }

    /* --- allocate adjacency arrays --------------------------------- */
    trans_from = (unsigned short **)malloc(MAX_K * sizeof(unsigned short *));
    trans_to   = (unsigned short **)malloc(MAX_K * sizeof(unsigned short *));
    for (int k = 0; k < MAX_K; ++k) {
        if (trans_cnt[k] == 0) {
            trans_from[k] = NULL;
            trans_to[k]   = NULL;
            continue;
        }
        trans_from[k] = (unsigned short *)malloc(trans_cnt[k] * sizeof(unsigned short));
        trans_to[k]   = (unsigned short *)malloc(trans_cnt[k] * sizeof(unsigned short));
    }

    /* --- second pass: fill adjacency arrays ----------------------- */
    size_t *curr = (size_t *)calloc(MAX_K, sizeof(size_t));
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            int key0 = (row_vals[i][0] << 6) | (row_vals[i][1] << 4)
                     | (row_vals[j][0] << 2) | (row_vals[j][1]);
            int e0   = expr_tbl[key0];
            int key1 = (row_vals[i][1] << 6) | (row_vals[i][2] << 4)
                     | (row_vals[j][1] << 2) | (row_vals[j][2]);
            if (expr_tbl[key1] != e0) continue;
            int key2 = (row_vals[i][2] << 6) | (row_vals[i][3] << 4)
                     | (row_vals[j][2] << 2) | (row_vals[j][3]);
            if (expr_tbl[key2] != e0) continue;
            int key3 = (row_vals[i][3] << 6) | (row_vals[i][4] << 4)
                     | (row_vals[j][3] << 2) | (row_vals[j][4]);
            if (expr_tbl[key3] != e0) continue;

            unsigned char k = (unsigned char)e0;
            size_t idx = curr[k]++;
            trans_from[k][idx] = (unsigned short)i;
            trans_to[k][idx]   = (unsigned short)j;
        }
    }
    free(curr);

    /* --- free temporary per‑row counters -------------------------- */
    for (int k = 0; k < MAX_K; ++k)
        free(perrow_cnt[k]);
    free(perrow_cnt);
}

/* ---------------------------------------------------------------- */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based.  Height H = n+1 rows. */
    idx_t H = n + 1;
    if (H < 2) return (seq_t)0;   /* safety for offset <1 */

    init_tables();

    static seq_t dp_prev[MAX_ROWS];
    static seq_t dp_next[MAX_ROWS];
    seq_t total = (seq_t)0;

    /* Iterate over all possible K values. */
    for (int k = 0; k < MAX_K; ++k) {
        if (trans_cnt[k] == 0) continue;   /* no valid transitions for this K */

        /* Start with any row. */
        for (int i = 0; i < MAX_ROWS; ++i) dp_prev[i] = (seq_t)1;

        /* Perform H‑1 transitions. */
        for (idx_t step = 1; step < H; ++step) {
            for (int i = 0; i < MAX_ROWS; ++i) dp_next[i] = (seq_t)0;

            size_t cnt = trans_cnt[k];
            unsigned short *from = trans_from[k];
            unsigned short *to   = trans_to[k];
            for (size_t e = 0; e < cnt; ++e) {
                seq_t val = dp_prev[from[e]];
                if (val)
                    dp_next[to[e]] = (seq_t)(dp_next[to[e]] + val);
            }

            for (int i = 0; i < MAX_ROWS; ++i) dp_prev[i] = dp_next[i];
        }

        /* Sum all sequences for this K. */
        for (int i = 0; i < MAX_ROWS; ++i)
            total = (seq_t)(total + dp_prev[i]);
    }

    return total;
}
