/*
 * A183578  Half the number of n X 3 0..3 arrays with each element equal to
 *          either the maximum or the minimum of its horizontal and vertical
 *          neighbors.
 * https://oeis.org/A183578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ½ × (# of n×3 arrays with entries in {0,1,2,3} such that
 *                each entry equals the maximum or the minimum of its
 *                existing horizontal and vertical neighbors).
 *
 * The implementation counts the total number of arrays via dynamic programming
 * on overlapping rows.  The count is then halved according to the sequence
 * definition.
 */
#include <stdint.h>
#include <stddef.h>

/* Helper macros */
#define ROWS 64          /* 4^3 possible rows */
#define IDX(a,b) ((a)*ROWS+(b))   /* index into 2‑D arrays */

/* Pre‑allocated tables */
static uint8_t ok_first[ROWS*ROWS];          /* row0 must be valid with down= row1 */
static uint8_t ok_last [ROWS*ROWS];          /* last row must be valid with up= row(prev) */
static uint8_t ok_mid  [ROWS*ROWS*ROWS];     /* middle row must be valid between two rows */
static uint8_t next_cnt[ROWS*ROWS];  /* number of outgoing transitions for each pair */
static uint8_t next_tbl[ROWS*ROWS][ROWS];    /* flattened adjacency lists */

/* Utility to extract a 0..3 value from a packed row */
static inline int get_val(int row, int col)
{
    return (row >> (2*col)) & 3;
}

/* Initialise the static tables once */
static void init_tables(void)
{
    static int inited = 0;
    if (inited) return;
    inited = 1;

    /* Pre‑compute ok_first and ok_last */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            /* row0 with only down neighbor r1 */
            int good = 1;
            for (int j = 0; j < 3 && good; ++j) {
                int minv = 4, maxv = -1;
                if (j > 0) {
                    int v = get_val(r0, j-1);
                    if (v < minv) minv = v;
                    if (v > maxv) maxv = v;
                }
                if (j < 2) {
                    int v = get_val(r0, j+1);
                    if (v < minv) minv = v;
                    if (v > maxv) maxv = v;
                }
                /* down neighbor always exists */
                int v = get_val(r1, j);
                if (v < minv) minv = v;
                if (v > maxv) maxv = v;
                if (get_val(r0, j) != minv && get_val(r0, j) != maxv)
                    good = 0;
            }
            ok_first[IDX(r0,r1)] = good;

            /* last row with only up neighbor r0 */
            good = 1;
            for (int j = 0; j < 3 && good; ++j) {
                int minv = 4, maxv = -1;
                if (j > 0) {
                    int v = get_val(r1, j-1);
                    if (v < minv) minv = v;
                    if (v > maxv) maxv = v;
                }
                if (j < 2) {
                    int v = get_val(r1, j+1);
                    if (v < minv) minv = v;
                    if (v > maxv) maxv = v;
                }
                /* up neighbor always exists */
                int v = get_val(r0, j);
                if (v < minv) minv = v;
                if (v > maxv) maxv = v;
                if (get_val(r1, j) != minv && get_val(r1, j) != maxv)
                    good = 0;
            }
            ok_last[IDX(r0,r1)] = good;
        }
    }

    /* Pre‑compute ok_mid and adjacency lists */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            uint8_t *list = next_tbl[IDX(a,b)];
            uint8_t count = 0;
            for (int c = 0; c < ROWS; ++c) {
                int good = 1;
                for (int j = 0; j < 3 && good; ++j) {
                    int minv = 4, maxv = -1;
                    if (j > 0) {
                        int v = get_val(b, j-1);
                        if (v < minv) minv = v;
                        if (v > maxv) maxv = v;
                    }
                    if (j < 2) {
                        int v = get_val(b, j+1);
                        if (v < minv) minv = v;
                        if (v > maxv) maxv = v;
                    }
                    /* up and down neighbors always exist */
                    int v = get_val(a, j);
                    if (v < minv) minv = v;
                    if (v > maxv) maxv = v;
                    v = get_val(c, j);
                    if (v < minv) minv = v;
                    if (v > maxv) maxv = v;
                    if (get_val(b, j) != minv && get_val(b, j) != maxv)
                        good = 0;
                }
                if (good) {
                    ok_mid[IDX(a,IDX(b,c))] = 1;
                    list[count++] = (uint8_t)c;
                }
            }
            next_cnt[IDX(a,b)] = count;
        }
    }
}

/* Count valid single rows (only horizontal neighbors) */
static seq_t count_single_rows(void)
{
    seq_t total = 0;
    for (int r = 0; r < ROWS; ++r) {
        int good = 1;
        for (int j = 0; j < 3 && good; ++j) {
            int minv = 4, maxv = -1;
            if (j > 0) {
                int v = get_val(r, j-1);
                if (v < minv) minv = v;
                if (v > maxv) maxv = v;
            }
            if (j < 2) {
                int v = get_val(r, j+1);
                if (v < minv) minv = v;
                if (v > maxv) maxv = v;
            }
            if (get_val(r, j) != minv && get_val(r, j) != maxv)
                good = 0;
        }
        if (good) total += (seq_t)1;
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return (seq_t)0;           /* undefined, return 0 */

    if (n == 1) {
        seq_t total = count_single_rows();
        return total / (seq_t)2;
    }

    /* DP arrays */
    static seq_t dp[ROWS*ROWS];
    static seq_t next[ROWS*ROWS];

    /* Initialise dp for pairs (row0,row1) with row0 valid with down neighbor */
    for (size_t i = 0; i < ROWS*ROWS; ++i) dp[i] = 0;
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            if (ok_first[IDX(r0,r1)])
                dp[IDX(r0,r1)] = (seq_t)1;
        }
    }

    /* Process intermediate rows (n-2 of them) */
    for (idx_t step = 2; step < n; ++step) {
        for (size_t i = 0; i < ROWS*ROWS; ++i) next[i] = 0;
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                seq_t val = dp[IDX(a,b)];
                if (val == 0) continue;
                uint8_t *list = next_tbl[IDX(a,b)];
                uint8_t cnt = next_cnt[IDX(a,b)];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int c = list[k];
                    next[IDX(b,c)] += val;
                }
            }
        }
        /* swap dp and next */
        for (size_t i = 0; i < ROWS*ROWS; ++i) dp[i] = next[i];
    }

    /* Final validation of last row (row n-1) with only up neighbor */
    seq_t total = 0;
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            if (ok_last[IDX(a,b)]) {
                total += dp[IDX(a,b)];
            }
        }
    }

    return total / (seq_t)2;
}
