/*
 * A207071  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A207071
    Number of 4×n binary arrays avoiding the horizontal patterns
      0 0 1 and 0 1 0,
    and avoiding the vertical patterns
      0 0 1 and 1 0 1.

    This is computed by a transfer‑matrix DP:
    * Only 8 column patterns satisfy the vertical constraints.
    * For each pair of successive columns we keep the number of ways
      to obtain that pair.
    * When a new column is added, for each previous pair we add the
      new column to all transitions that do not create forbidden
      horizontal triples.
    * The count for n columns is the sum over all pair states after
      processing n columns.
*/
#include <stdbool.h>

#define ALLOWED 8

/* allowed column patterns (4 bits) – vertical constraints only */
static const int allowed[ALLOWED] = {
    0,  /* 0000 */
    4,  /* 0100 */
    6,  /* 0110 */
    7,  /* 0111 */
    8,  /* 1000 */
    12, /* 1100 */
    14, /* 1110 */
    15  /* 1111 */
};

static bool trans_ok[ALLOWED][ALLOWED][ALLOWED];
static bool tables_ready = false;

/* Build the transition table once per program lifetime */
static void build_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;
    for (int i = 0; i < ALLOWED; ++i) {
        int pi = allowed[i];
        for (int j = 0; j < ALLOWED; ++j) {
            int pj = allowed[j];
            for (int k = 0; k < ALLOWED; ++k) {
                int pk = allowed[k];
                bool ok = true;
                for (int r = 0; r < 4; ++r) {
                    int a = (pi >> r) & 1;
                    int b = (pj >> r) & 1;
                    int c = (pk >> r) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 0 && b == 1 && c == 0)) {
                        ok = false;
                        break;
                    }
                }
                trans_ok[i][j][k] = ok;
            }
        }
    }
}

/* Main function as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not defined by OEIS, just safety */
    if (n == 1) return 8;          /* only vertical constraints */
    if (n == 2) return 64;         /* pairs of allowed columns */

    build_tables();

    /* dp[i][j] = number of ways ending with columns (i,j) */
    seq_t dp[ALLOWED][ALLOWED];
    seq_t next[ALLOWED][ALLOWED];

    /* initialise after two columns: one way for each pair */
    for (int i = 0; i < ALLOWED; ++i)
        for (int j = 0; j < ALLOWED; ++j)
            dp[i][j] = 1;

    /* process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero next array */
        for (int i = 0; i < ALLOWED; ++i)
            for (int j = 0; j < ALLOWED; ++j)
                next[i][j] = 0;

        /* transition */
        for (int i = 0; i < ALLOWED; ++i)
            for (int j = 0; j < ALLOWED; ++j) {
                seq_t cnt = dp[i][j];
                if (!cnt) continue;
                for (int k = 0; k < ALLOWED; ++k) {
                    if (trans_ok[i][j][k])
                        next[j][k] += cnt;
                }
            }

        /* copy next to dp for next iteration */
        for (int i = 0; i < ALLOWED; ++i)
            for (int j = 0; j < ALLOWED; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum over all states to obtain total for n columns */
    seq_t result = 0;
    for (int i = 0; i < ALLOWED; ++i)
        for (int j = 0; j < ALLOWED; ++j)
            result += dp[i][j];

    return result;
}
