/*
 * A203349  Number of (n+2)X4 binary arrays avoiding patterns 010 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A203349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+2)X4 binary arrays avoiding patterns 010 and 101 in
 *            rows, columns and nw-to-se diagonals, read as a(n) = a(n+2 rows)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses a transfer‑matrix DP with a state consisting of the
    two most recent rows (4 bits each).  Transitions are pre‑computed for
    all 16×16×16 triples.  Each transition checks the horizontal,
    vertical and diagonal constraints.  The DP iterates n times (since
    the matrix has n+2 rows).  The total number of matrices is the sum
    of all final states.  This is O(n) and easily fast enough for n
    up to about 10 000. */

#include <stddef.h>
#include <stdbool.h>

static const unsigned char GOOD_ROW[16] = {
    /* 0b0000 */ 1, /* 0b0001 */ 1, /* 0b0010 */ 0, /* 0b0011 */ 1,
    /* 0b0100 */ 0, /* 0b0101 */ 0, /* 0b0110 */ 1, /* 0b0111 */ 1,
    /* 0b1000 */ 1, /* 0b1001 */ 1, /* 0b1010 */ 0, /* 0b1011 */ 0,
    /* 0b1100 */ 1, /* 0b1101 */ 0, /* 0b1110 */ 1, /* 0b1111 */ 1
};

/* Transition validity: trans[r0][r1][r2] == 1 iff the triple of rows
   satisfies all constraints.  Indexed by 4‑bit integers. */
static unsigned char TRANS[16][16][16];
static bool TABLES_INITIALIZED = false;

static void init_tables(void)
{
    if (TABLES_INITIALIZED) return;
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            for (int r2 = 0; r2 < 16; ++r2) {
                unsigned char ok = 1;
                if (!GOOD_ROW[r0] || !GOOD_ROW[r1] || !GOOD_ROW[r2]) {
                    ok = 0;
                } else {
                    /* Horizontal check for r2 */
                    unsigned int h1 = ((r2 >> 0) & 7);
                    unsigned int h2 = ((r2 >> 1) & 7);
                    if (h1 == 0b010 || h1 == 0b101 ||
                        h2 == 0b010 || h2 == 0b101) {
                        ok = 0;
                    }
                    /* Vertical check for each column */
                    if (ok) {
                        for (int j = 0; j < 4; ++j) {
                            unsigned int pat = ((r0 >> j) & 1) << 2 |
                                               ((r1 >> j) & 1) << 1 |
                                               ((r2 >> j) & 1);
                            if (pat == 0b010 || pat == 0b101) {
                                ok = 0;
                                break;
                            }
                        }
                    }
                    /* Diagonal check */
                    if (ok) {
                        /* j = 2 : bits (r0[0], r1[1], r2[2]) */
                        unsigned int pat = ((r0 >> 0) & 1) << 2 |
                                           ((r1 >> 1) & 1) << 1 |
                                           ((r2 >> 2) & 1);
                        if (pat == 0b010 || pat == 0b101) ok = 0;
                        /* j = 3 : bits (r0[1], r1[2], r2[3]) */
                        if (ok) {
                            pat = ((r0 >> 1) & 1) << 2 |
                                  ((r1 >> 2) & 1) << 1 |
                                  ((r2 >> 3) & 1);
                            if (pat == 0b010 || pat == 0b101) ok = 0;
                        }
                    }
                }
                TRANS[r0][r1][r2] = ok;
            }
        }
    }
    TABLES_INITIALIZED = true;
}

seq_t a(idx_t n)
{
    /* n >= 1, matrix has n+2 rows */
    init_tables();

    /* dp_cur[r0][r1] = number of ways to reach state (r0,r1) after
       current number of rows processed. */
    seq_t dp_cur[16][16] = {0};
    seq_t dp_next[16][16] = {0};

    /* Initial two rows can be any pair of good rows (no other constraints) */
    for (int r0 = 0; r0 < 16; ++r0) if (GOOD_ROW[r0])
        for (int r1 = 0; r1 < 16; ++r1) if (GOOD_ROW[r1])
            dp_cur[r0][r1] = 1;

    /* Add rows n times; after n additions we will have n+2 rows */
    for (idx_t step = 0; step < n; ++step) {
        /* clear dp_next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp_next[i][j] = 0;

        for (int r0 = 0; r0 < 16; ++r0) {
            for (int r1 = 0; r1 < 16; ++r1) {
                seq_t cur = dp_cur[r0][r1];
                if (cur == 0) continue;
                for (int r2 = 0; r2 < 16; ++r2) {
                    if (!TRANS[r0][r1][r2]) continue;
                    dp_next[r1][r2] += cur;
                }
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            total += dp_cur[r0][r1];

    return total;
}
