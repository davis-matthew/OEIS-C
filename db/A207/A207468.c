/*
 * A207468  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 44, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming on a 256‑state automaton representing the
   last two columns of a 4×n binary array.  States are 4‑bit columns; a
   transition from (a,b) to (b,c) is allowed iff c satisfies the vertical
   constraint and for every row r the triple (a_r,b_r,c_r) is not 000 or 011. */

#include <stdbool.h>

static int ALLOWED[10];
static int ALLOWED_COUNT = 0;
static int TRANS[256][10];
static int TRANS_CNT[256];
static bool INIT_DONE = false;

/* test that a 4‑bit column has no vertical 010 or 101 patterns */
static bool valid_vertical(int col)
{
    for (int r = 0; r <= 1; ++r) {
        int b1 = (col >> r) & 1;
        int b2 = (col >> (r + 1)) & 1;
        int b3 = (col >> (r + 2)) & 1;
        if ((b1 == 0 && b2 == 1 && b3 == 0) ||
            (b1 == 1 && b2 == 0 && b3 == 1))
            return false;
    }
    return true;
}

/* test that extending state (a,b) by column c does not create forbidden
   horizontal 000 or 011 patterns in any row */
static bool valid_horizontal(int a, int b, int c)
{
    for (int r = 0; r < 4; ++r) {
        int ba = (a >> r) & 1;  /* prevprev */
        int bb = (b >> r) & 1;  /* prev     */
        int bc = (c >> r) & 1;  /* new      */
        if ((ba == 0 && bb == 0 && bc == 0) ||
            (ba == 0 && bb == 1 && bc == 1))
            return false;
    }
    return true;
}

/* one‑time initialization of allowed columns and transition table */
static void init(void)
{
    if (INIT_DONE) return;
    INIT_DONE = true;

    int cnt = 0;
    for (int col = 0; col < 16; ++col)
        if (valid_vertical(col))
            ALLOWED[cnt++] = col;
    ALLOWED_COUNT = cnt;

    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b) {
            int idx = a * 16 + b;
            int tcnt = 0;
            for (int k = 0; k < ALLOWED_COUNT; ++k) {
                int c = ALLOWED[k];
                if (valid_horizontal(a, b, c))
                    TRANS[idx][tcnt++] = c;
            }
            TRANS_CNT[idx] = tcnt;
        }
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init();

    if (n == 0) return 0;                     /* protective, not used */
    if (n == 1) return (seq_t)ALLOWED_COUNT;  /* only vertical constraints */
    if (n == 2) return (seq_t)ALLOWED_COUNT *
                     (seq_t)ALLOWED_COUNT;    /* any pair of allowed columns */

    static seq_t dp_cur[256];
    static seq_t dp_next[256];

    /* initialize counts for column length 2 */
    for (int i = 0; i < 256; ++i) dp_cur[i] = 0;
    for (int ai = 0; ai < ALLOWED_COUNT; ++ai)
        for (int bi = 0; bi < ALLOWED_COUNT; ++bi)
            dp_cur[ALLOWED[ai] * 16 + ALLOWED[bi]] = 1;

    /* extend to length n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int base = s % 16;               /* previous column b */
            int tcnt = TRANS_CNT[s];
            for (int j = 0; j < tcnt; ++j) {
                int c = TRANS[s][j];
                int t = base * 16 + c;
                dp_next[t] += val;
            }
        }
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* total number of 4×n arrays */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += dp_cur[i];
    return total;
}
