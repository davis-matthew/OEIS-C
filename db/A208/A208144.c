/*
 * A208144  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   is_col_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A208144 – Number of 5×n 0..1 arrays avoiding
   horizontal patterns 0 0 1 and 0 1 1 and vertical patterns 0 0 1 and 1 0 1.
   The sequence is computed by a transfer‑matrix dynamic programming
   over the 10 admissible column patterns. 
*/

#include <stddef.h>
#include <string.h>
#include "../utils/is_col_ok.h"

static unsigned char allowed[10];          /* 10 admissible columns (5‑bit mask) */
static unsigned char trans[10][10][10];    /* transition table */
static int init_flag = 0;                  /* one‑time initialization */

/* one‑time setup: list admissible columns and build transition table */
static void init(void)
{
    int cnt = 0;
    for (int pat = 0; pat < 32; ++pat) {
        if (is_col_ok(pat))
            allowed[cnt++] = (unsigned char)pat;
    }
    /* there must be exactly 10 admissible columns */
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            for (int c = 0; c < 10; ++c) {
                int ok = 1;
                for (int r = 0; r < 5; ++r) {
                    int ba = (allowed[a] >> r) & 1;
                    int bb = (allowed[b] >> r) & 1;
                    int bc = (allowed[c] >> r) & 1;
                    if ((ba == 0 && bb == 0 && bc == 1) ||      /* 0 0 1 */
                        (ba == 0 && bb == 1 && bc == 1)) {     /* 0 1 1 */
                        ok = 0;
                        break;
                    }
                }
                trans[a][b][c] = ok;
            }
    init_flag = 1;
}

/* ------------------------------------------------------------ */
/* Main function requested by the problem statement            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_flag) init();

    if (n == 0) return (seq_t)0;          /* out of range, return 0 */
    if (n == 1) return (seq_t)10;         /* single column */
    if (n == 2) return (seq_t)100;        /* any pair of columns */

    seq_t dp[10][10];
    seq_t newdp[10][10];

    /* initialise for length 2: all pairs are allowed */
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            dp[i][j] = (seq_t)1;

    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                newdp[i][j] = (seq_t)0;

        for (int a = 0; a < 10; ++a)
            for (int b = 0; b < 10; ++b) {
                seq_t v = dp[a][b];
                if (v == 0) continue;
                for (int c = 0; c < 10; ++c)
                    if (trans[a][b][c])
                        newdp[b][c] += v;
            }

        /* copy newdp into dp for next iteration */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* sum over all ending pairs to obtain the total count */
    seq_t total = (seq_t)0;
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            total += dp[a][b];

    return total;
}
