/*
 * A204757  Number of (n+1)X3 0..5 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A204757
    Number of (n+1)×3 0..5 arrays with the permanents of all 2×2
    subblocks equal and nonzero.
    The value a(n) is the number of directed walks of length n in a
    directed graph whose vertices are the 6³ possible rows.
    An edge (r1 → r2, p) exists iff the permanent of the
    2×2 blocks [r1[0] r1[1]; r2[0] r2[1]] and [r1[1] r1[2]; r2[1] r2[2]]
    is equal to p>0.  The answer is the number of such walks regardless
    of the final row and the permanent value.
    The algorithm pre‑computes all edges once and then uses dynamic
    programming in O(n·E) time, where E≤46656.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>   /* for bool definition */

#define ROWS 216          /* 6³ possible rows */
#define MAXP 50           /* maximum permanent value (5*5+5*5) */

static bool built = false;
static int  src_start[ROWS + 1];
static int  src_len  [ROWS ];
static int  dests[ROWS * ROWS];
static int  ps   [ROWS * ROWS];

static void build_graph(void)
{
    /* step 1 – count edges per source */
    for (int src = 0; src < ROWS; ++src)
        src_len[src] = 0;

    int digit[ROWS][3];
    for (int r = 0; r < ROWS; ++r) {
        digit[r][0] = r / 36;
        digit[r][1] = (r / 6) % 6;
        digit[r][2] = r % 6;
    }

    for (int u = 0; u < ROWS; ++u) {
        int a = digit[u][0], b = digit[u][1], c = digit[u][2];
        for (int v = 0; v < ROWS; ++v) {
            int a2 = digit[v][0], b2 = digit[v][1], c2 = digit[v][2];
            int P1 = a * b2 + b * a2;
            int P2 = b * c2 + c * b2;
            if (P1 > 0 && P1 == P2)
                ++src_len[u];
        }
    }

    /* step 2 – compute prefix sums to obtain start indices */
    src_start[0] = 0;
    for (int src = 0; src < ROWS; ++src)
        src_start[src + 1] = src_start[src] + src_len[src];

    /* step 3 – fill dest and p arrays */
    for (int src = 0; src < ROWS; ++src) {
        int a = digit[src][0], b = digit[src][1], c = digit[src][2];
        int base = src_start[src];
        int pos  = 0;
        for (int dst = 0; dst < ROWS; ++dst) {
            int a2 = digit[dst][0], b2 = digit[dst][1], c2 = digit[dst][2];
            int P1 = a * b2 + b * a2;
            int P2 = b * c2 + c * b2;
            if (P1 > 0 && P1 == P2) {
                dests[base + pos] = dst;
                ps    [base + pos] = P1;   /* same as P2 */
                ++pos;
            }
        }
    }

    built = true;
}

/*  Dynamic programming arrays: dp[2][MAXP+1][ROWS]  */
static seq_t dp[2][MAXP + 1][ROWS];

seq_t a(idx_t n)
{
    if (!built)
        build_graph();

    /* Clear DP tables */
    for (int p = 1; p <= MAXP; ++p)
        for (int r = 0; r < ROWS; ++r)
            dp[0][p][r] = 0, dp[1][p][r] = 0;

    int cur  = 0;
    int next = 1;

    /* Initialise for paths of length 1 (one edge) */
    for (int src = 0; src < ROWS; ++src) {
        int base = src_start[src];
        int len  = src_len[src];
        for (int k = 0; k < len; ++k) {
            int dest = dests[base + k];
            int p    = ps    [base + k];
            dp[cur][p][dest] += (seq_t)1;
        }
    }

    /* If n == 1 we are done */
    if (n > 1) {
        /* For each additional edge */
        for (idx_t step = 1; step < n; ++step) {
            /* zero dp[next] */
            for (int p = 1; p <= MAXP; ++p)
                for (int r = 0; r < ROWS; ++r)
                    dp[next][p][r] = 0;

            /* propagate edges */
            for (int src = 0; src < ROWS; ++src) {
                int base = src_start[src];
                int len  = src_len[src];
                for (int k = 0; k < len; ++k) {
                    int dest = dests[base + k];
                    int p    = ps    [base + k];
                    seq_t val = dp[cur][p][src];
                    if (val)
                        dp[next][p][dest] += val;
                }
            }
            /* swap */
            int tmp  = cur;
            cur     = next;
            next    = tmp;
        }
    }

    /* sum all paths of length n */
    seq_t total = 0;
    for (int p = 1; p <= MAXP; ++p)
        for (int r = 0; r < ROWS; ++r)
            total += dp[cur][p][r];

    return total;
}
