/*
 * A230691  Number of (n+2)X(2+2) 0..2 arrays x(i,j) with each element
 *          horizontally, vertically, diagonally or antidiagonally next to at
 *          least two elements with value (x(i,j)+1) mod 3, and upper left
 *          element zero.
 * https://oeis.org/A230691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230691
 * Number of (n+2)×4 arrays x(i,j) with 0≤x≤2 that satisfy:
 *  each element must have at least two neighbors (horiz., vert.,
 *   diag., or anti‑diag.) equal to (x+1) mod 3,
 *  the upper‑left element is 0.
 * Implementation: transfer‑matrix DP over rows.
 * The state is a pair of consecutive rows.  Pre‑computed tables
 * encode which row pairs are allowed and which rows may be the
 * bottommost one.
 */

#include <stdbool.h>          /* for bool */

#define ROWS 81   /* 3^4 possible rows of length 4 */

static int  row_val[ROWS][4];         /* digits of a row */
static int  next_row[ROWS][ROWS][ROWS];  /* allowed successors */
static unsigned char next_cnt[ROWS][ROWS];/* number of successors */
static unsigned char bottom_ok[ROWS][ROWS];/* r1 valid as bottom with r0 above */
static bool ready = false;

/* convert a row code (0..80) to its 4 digits */
static void init_row_val(void)
{
    for (int code = 0; code < ROWS; ++code) {
        int x = code;
        for (int c = 0; c < 4; ++c) {
            row_val[code][c] = x % 3;
            x /= 3;
        }
    }
}

/* test whether a row q can sit between prev and next (prev/next may be -1) */
static bool check_row(int prev, int curr, int next)
{
    for (int c = 0; c < 4; ++c) {
        int val  = row_val[curr][c];
        int want = (val + 1) % 3;
        int cnt  = 0;
        /* above */
        if (prev != -1) {
            if (c > 0   && row_val[prev][c-1] == want) ++cnt;
            if (          row_val[prev][c]   == want) ++cnt;
            if (c < 3   && row_val[prev][c+1] == want) ++cnt;
        }
        /* same row */
        if (c > 0 && row_val[curr][c-1] == want) ++cnt;
        if (c < 3 && row_val[curr][c+1] == want) ++cnt;
        /* below */
        if (next != -1) {
            if (c > 0   && row_val[next][c-1] == want) ++cnt;
            if (          row_val[next][c]   == want) ++cnt;
            if (c < 3   && row_val[next][c+1] == want) ++cnt;
        }
        if (cnt < 2) return false;
    }
    return true;
}

/* initialise all pre‑computed tables */
static void init_tables(void)
{
    if (ready) return;
    ready = true;

    init_row_val();

    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            unsigned char t = 0;
            for (int r = 0; r < ROWS; ++r) {
                if (check_row(p, q, r)) {
                    next_row[p][q][t++] = r;
                }
            }
            next_cnt[p][q] = t;
            bottom_ok[p][q] = (unsigned char)check_row(p, q, -1);
        }
    }
}

/* sequence entry function */
/* external linkage required */
seq_t a(idx_t n)
{
    /* number of rows = n+2, transitions after the first two rows = n */
    idx_t steps = n;               /* transitions to process */

    init_tables();

    static seq_t dp_cur[ROWS][ROWS];
    static seq_t dp_nxt[ROWS][ROWS];

    /* clear dp_cur */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp_cur[i][j] = 0;

    /* initialise first two rows: top row has no upper neighbours,
       and its leftmost entry must be 0 */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        if (row_val[r0][0] != 0) continue;          /* upper‑left zero */
        for (int r1 = 0; r1 < ROWS; ++r1) {
            if (check_row(-1, r0, r1))
                ++dp_cur[r0][r1];
        }
    }

    /* transition for the remaining rows */
    for (idx_t s = 1; s <= steps; ++s) {
        /* clear dp_nxt */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp_nxt[i][j] = 0;

        for (int p = 0; p < ROWS; ++p) {
            for (int q = 0; q < ROWS; ++q) {
                seq_t cur = dp_cur[p][q];
                if (cur == 0) continue;
                unsigned char t = next_cnt[p][q];
                for (unsigned char k = 0; k < t; ++k) {
                    int r = next_row[p][q][k];
                    dp_nxt[q][r] += cur;
                }
            }
        }
        /* copy dp_nxt into dp_cur for next iteration */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp_cur[i][j] = dp_nxt[i][j];
    }

    /* final result: bottom row must be valid with its above row */
    seq_t res = 0;
    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            if (bottom_ok[p][q])
                res += dp_cur[p][q];
        }
    }
    return res;
}
