/*
 * A202611  Number of (n+2)X5 binary arrays avoiding patterns 001 and 010 in
 *          rows and columns.
 * https://oeis.org/A202611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+2)×5 binary arrays avoiding 001 and 010 in rows and columns.
 * The function enumerates all admissible rows (length‑5 binary strings) and
 * uses dynamic programming over the rows.  The table of allowed vertical
 * triples is pre‑computed once.  Complexity is O(n·R³) where R ≤ 32 is the
 * number of admissible rows; this easily meets the time limits for n up to
 * 10 000. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* pre‑computed data */
    static bool ready = false;
    static unsigned char rowVals[32];
    static unsigned char rowCount;
    static unsigned char nextK[32][32][32];
    static unsigned char nextK_len[32][32];
    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* initialise the admissible rows and transitions only once */
    if (!ready) {
        unsigned char cnt = 0;
        for (unsigned char val = 0; val < 32; ++val) {
            bool ok = true;
            /* check horizontal patterns 001 and 010 */
            for (unsigned char p = 0; p <= 2; ++p) {
                unsigned char b0 = (val >> p) & 1;
                unsigned char b1 = (val >> (p + 1)) & 1;
                unsigned char b2 = (val >> (p + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) rowVals[cnt++] = val;
        }
        rowCount = cnt;

        /* vertical triples */
        for (unsigned char i = 0; i < rowCount; ++i) {
            for (unsigned char j = 0; j < rowCount; ++j) {
                unsigned char len = 0;
                for (unsigned char k = 0; k < rowCount; ++k) {
                    bool ok = true;
                    for (unsigned char c = 0; c < 5; ++c) {
                        unsigned char bi = (rowVals[i] >> c) & 1;
                        unsigned char bj = (rowVals[j] >> c) & 1;
                        unsigned char bk = (rowVals[k] >> c) & 1;
                        if ((bi == 0 && bj == 0 && bk == 1) ||
                            (bi == 0 && bj == 1 && bk == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) nextK[i][j][len++] = k;
                }
                nextK_len[i][j] = len;
            }
        }
        ready = true;
    }

    /* number of rows in the array */
    idx_t rows = n + 2;
    if (rows <= 1) {                 /* single row */
        seq_t sum = 0;
        for (unsigned char i = 0; i < rowCount; ++i) sum += 1;
        return sum;
    }
    if (rows == 2) {                 /* two rows: any pair */
        seq_t sum = 0;
        for (unsigned char i = 0; i < rowCount; ++i)
            for (unsigned char j = 0; j < rowCount; ++j)
                sum += 1;
        return sum;
    }

    /* initialise dp for the first two rows */
    for (unsigned char i = 0; i < rowCount; ++i)
        for (unsigned char j = 0; j < rowCount; ++j)
            dp[i][j] = 1;

    /* extend the array row by row */
    for (idx_t step = 3; step <= rows; ++step) {
        /* clear ndp */
        for (unsigned char i = 0; i < rowCount; ++i)
            for (unsigned char j = 0; j < rowCount; ++j)
                ndp[i][j] = 0;

        for (unsigned char i = 0; i < rowCount; ++i) {
            for (unsigned char j = 0; j < rowCount; ++j) {
                seq_t val = dp[i][j];
                if (val == 0) continue;
                unsigned char len = nextK_len[i][j];
                for (unsigned char t = 0; t < len; ++t) {
                    unsigned char k = nextK[i][j][t];
                    ndp[j][k] += val;
                }
            }
        }

        /* copy back */
        for (unsigned char i = 0; i < rowCount; ++i)
            for (unsigned char j = 0; j < rowCount; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all possibilities for the last two rows */
    seq_t result = 0;
    for (unsigned char i = 0; i < rowCount; ++i)
        for (unsigned char j = 0; j < rowCount; ++j)
            result += dp[i][j];

    return result;
}
