/*
 * A238294  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A238294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 such that for
 *            every 2×2 subblock the quantity max + upper‑median –
 *            lower‑median differs from its horizontal and vertical neighbors
 *            by exactly one. The values are computed by dynamic programming
 *            over rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   diff8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/diff8.h"

/* Pre‑computed tables for all 64 rows */
static unsigned char v1[64][64];
static unsigned char v2[64][64];
static bool horiz_ok[64][64];
static int transCnt[4096];
static int transNext[4096][64];
static bool tables_ready = false;

/* Build all tables once */
static void build_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;

    /* digits of rows */
    unsigned char digits[64][3];
    for (int r = 0; r < 64; ++r) {
        digits[r][0] = (unsigned char)(r & 3);
        digits[r][1] = (unsigned char)((r >> 2) & 3);
        digits[r][2] = (unsigned char)((r >> 4) & 3);
    }

    /* compute v1, v2, horizontal_ok for each pair of rows */
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            unsigned char a[4], b[4];
            /* first subblock: columns 0,1 */
            a[0] = digits[r0][0];
            a[1] = digits[r0][1];
            a[2] = digits[r1][0];
            a[3] = digits[r1][1];
            /* second subblock: columns 1,2 */
            b[0] = digits[r0][1];
            b[1] = digits[r0][2];
            b[2] = digits[r1][1];
            b[3] = digits[r1][2];

            /* sort a */
            for (int i = 0; i < 3; ++i)
                for (int j = i + 1; j < 4; ++j)
                    if (a[i] > a[j]) {
                        unsigned char t = a[i];
                        a[i] = a[j];
                        a[j] = t;
                    }
            /* sort b */
            for (int i = 0; i < 3; ++i)
                for (int j = i + 1; j < 4; ++j)
                    if (b[i] > b[j]) {
                        unsigned char t = b[i];
                        b[i] = b[j];
                        b[j] = t;
                    }

            unsigned char mv1 = a[3], lm1 = a[1], um1 = a[2];
            unsigned char mv2 = b[3], lm2 = b[1], um2 = b[2];
            v1[r0][r1] = mv1 + um1 - lm1;
            v2[r0][r1] = mv2 + um2 - lm2;
            horiz_ok[r0][r1] = diff8(v1[r0][r1], v2[r0][r1]) == 1;
        }
    }

    /* build transition lists */
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            int sid = r0 * 64 + r1;
            int cnt = 0;
            if (!horiz_ok[r0][r1]) { transCnt[sid] = 0; continue; }
            for (int r2 = 0; r2 < 64; ++r2) {
                if (diff8(v1[r0][r1], v1[r1][r2]) != 1) continue;
                if (diff8(v2[r0][r1], v2[r1][r2]) != 1) continue;
                if (!horiz_ok[r1][r2]) continue;
                transNext[sid][cnt++] = r2;
            }
            transCnt[sid] = cnt;
        }
    }
}

/* Dynamic programming arrays: use static to avoid reallocation */
static seq_t dp[64 * 64];
static seq_t newdp[64 * 64];

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset */

    build_tables();

    idx_t rows = n + 1;              /* number of rows in the array */

    /* initialise dp with all valid pairs of first two rows */
    for (size_t i = 0; i < 64ULL * 64ULL; ++i) dp[i] = 0;
    for (int r0 = 0; r0 < 64; ++r0)
        for (int r1 = 0; r1 < 64; ++r1)
            if (horiz_ok[r0][r1])
                dp[(size_t)r0 * 64 + r1] = 1;

    /* add remaining rows one by one */
    for (idx_t step = 3; step <= rows; ++step) {
        for (size_t i = 0; i < 64ULL * 64ULL; ++i) newdp[i] = 0;
        for (int r0 = 0; r0 < 64; ++r0) {
            for (int r1 = 0; r1 < 64; ++r1) {
                seq_t cur = dp[(size_t)r0 * 64 + r1];
                if (cur == 0) continue;
                int sid = r0 * 64 + r1;
                int cnt = transCnt[sid];
                for (int k = 0; k < cnt; ++k) {
                    int r2 = transNext[sid][k];
                    newdp[(size_t)r1 * 64 + r2] += cur;
                }
            }
        }
        for (size_t i = 0; i < 64ULL * 64ULL; ++i) dp[i] = newdp[i];
    }

    /* total number of arrays is the sum of all dp entries at the last step */
    seq_t total = 0;
    for (size_t i = 0; i < 64ULL * 64ULL; ++i) total += dp[i];
    return total;
}
