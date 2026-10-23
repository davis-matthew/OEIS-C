/*
 * A185469  Number of (n+2)X3 0..2 arrays with each 3X3 subblock having rows
 *          and columns in lexicographically nondecreasing order.
 * https://oeis.org/A185469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over rows.  
   For a (n+2)×3 array over {0,1,2} each 3×3 subblock must
   have rows and columns lexicographically nondecreasing.
   The DP keeps the last two rows and counts the extensions that
   satisfy the 3×3 condition.  */

#include <stdbool.h>
#include <stdint.h>

#define ROWS 27  /* 3^3 possible rows of length 3 over {0,1,2} */

static bool initialized = false;
/* decoded digits of each row pattern: decode[p][j] is digit at column j (0..2) */
static uint8_t decode[ROWS][3];
/* lexicographic ≤ for row patterns */
static bool lex_le[ROWS][ROWS];
/* allowed next rows for each pair (prev2, prev1) */
static int  allowed[ROWS][ROWS][ROWS];
static unsigned char allowed_cnt[ROWS][ROWS];

/* one‑time initialization of all tables */
static void init_tables(void)
{
    if (initialized) return;

    /* decode each pattern to its three digits */
    for (int p = 0; p < ROWS; ++p) {
        int x = p;
        decode[p][2] = x % 3; x /= 3;
        decode[p][1] = x % 3; x /= 3;
        decode[p][0] = x % 3;
    }

    /* lexicographic comparison table for row patterns */
    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            bool le = true;
            for (int j = 0; j < 3; ++j) {
                if (decode[p][j] < decode[q][j]) break;
                if (decode[p][j] > decode[q][j]) { le = false; break; }
            }
            lex_le[p][q] = le;
        }
    }

    /* pre‑compute allowed third rows for each pair (p,q) */
    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            allowed_cnt[p][q] = 0;
            if (!lex_le[p][q]) continue;           /* rows must be ordered p ≤ q */

            for (int cur = 0; cur < ROWS; ++cur) {
                if (!lex_le[q][cur]) continue;       /* q ≤ cur */

                /* build the three column vectors of the 3×3 subblock
                   and test their lexicographic order */
                int v[3];
                for (int j = 0; j < 3; ++j)
                    v[j] = decode[p][j] * 9 + decode[q][j] * 3 + decode[cur][j];

                /* require col₀ ≤ col₁ ≤ col₂ lexicographically */
                if (!lex_le[v[0]][v[1]]) continue;
                if (!lex_le[v[1]][v[2]]) continue;

                allowed[p][q][allowed_cnt[p][q]++] = cur;
            }
        }
    }

    initialized = true;
}

/* sequence function required by the test harness */
seq_t a(idx_t n)
{
    /* offset 1: for n < 1 the value is undefined; return 0 */
    if (n < 1) return (seq_t)0;

    init_tables();

    /* Heights of the array = n + 2 rows */
    idx_t H = n + 2;

    /* DP arrays: dp[p][q] = number of ways whose last two rows are p, q */
    static seq_t dp[ROWS][ROWS];
    static seq_t newdp[ROWS][ROWS];

    /* initial state: any pair of first two rows */
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q)
            dp[p][q] = 1;

    /* add rows from the third up to the H‑th */
    for (idx_t row = 3; row <= H; ++row) {
        /* clear newdp */
        for (int p = 0; p < ROWS; ++p)
            for (int q = 0; q < ROWS; ++q)
                newdp[p][q] = 0;

        /* transition */
        for (int p = 0; p < ROWS; ++p) {
            for (int q = 0; q < ROWS; ++q) {
                seq_t val = dp[p][q];
                if (val == 0) continue;
                unsigned char cnt = allowed_cnt[p][q];
                for (unsigned char k = 0; k < cnt; ++k) {
                    int cur = allowed[p][q][k];
                    newdp[q][cur] += val;
                }
            }
        }

        /* move newdp to dp for next iteration */
        for (int p = 0; p < ROWS; ++p)
            for (int q = 0; q < ROWS; ++q)
                dp[p][q] = newdp[p][q];
    }

    /* sum over all possible last two rows */
    seq_t ans = 0;
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q)
            ans += dp[p][q];

    return ans;
}
