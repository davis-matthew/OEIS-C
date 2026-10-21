/*
 * A235579  Number of (n+1)X(1+1) 0..3 arrays with the difference of the upper
 *          median and minimum value of each 2X2 subblock in lexicographically
 *          nondecreasing order rowwise and columnwise.
 * https://oeis.org/A235579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+1)×2 arrays with values 0..3 such that the
 * difference of the upper median and minimum of each 2×2 subblock
 * form a non‑decreasing sequence.
 * The values are computed by a simple DP over 16 row‑pairs
 * and the 4 possible difference values.  The recurrence is
 * a(n) = Σ_{prevRow,ld} Σ_{curRow}
 *        dp[i-1][prevRow][ld]           (only if diff(prevRow,curRow) ≥ ld).
 * initial step i=1 counts all 16² 2×row arrays → a(1)=256. */

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n < 1) return 0;

    /* precompute diff[prev][cur] once */
    static unsigned char diff[16][16];
    static int initialized = 0;
    if (!initialized) {
        for (int p = 0; p < 16; ++p) {
            int a0 = (p >> 2) & 3;
            int b0 = p & 3;
            for (int q = 0; q < 16; ++q) {
                int a1 = (q >> 2) & 3;
                int b1 = q & 3;
                int v[4] = { a0, b0, a1, b1 };
                /* simple bubble sort for 4 elements */
                for (int i = 0; i < 3; ++i)
                    for (int j = i + 1; j < 4; ++j)
                        if (v[i] > v[j]) {
                            int tmp = v[i];
                            v[i] = v[j];
                            v[j] = tmp;
                        }
                diff[p][q] = (unsigned char)(v[2] - v[0]);   /* upper median - minimum */
            }
        }
        initialized = 1;
    }

    /* a(1) is trivial: all 16×16 arrays */
    if (n == 1) return (seq_t)256;

    /* DP tables: dp[row][d] = count of arrays ending with row pair `row`
     * and last diff value `d` (0..3).  Use two arrays for current/next. */
    seq_t dp[16][4];
    seq_t ndp[16][4];

    /* initialize with the first transition (from the first to the second row) */
    for (int i = 0; i < 16; ++i)
        for (int d = 0; d < 4; ++d)
            dp[i][d] = 0;

    for (int p0 = 0; p0 < 16; ++p0) {
        for (int p1 = 0; p1 < 16; ++p1) {
            unsigned char d = diff[p0][p1];
            dp[p1][d] += (seq_t)1;
        }
    }

    /* perform the remaining transitions (total n transitions, we already did 1) */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear next dp */
        for (int i = 0; i < 16; ++i)
            for (int d = 0; d < 4; ++d)
                ndp[i][d] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int ld = 0; ld < 4; ++ld) {
                seq_t val = dp[prev][ld];
                if (val == 0) continue;
                for (int cur = 0; cur < 16; ++cur) {
                    unsigned char d = diff[prev][cur];
                    if (d >= ld) {
                        ndp[cur][d] += val;
                    }
                }
            }
        }

        /* copy back to dp */
        for (int i = 0; i < 16; ++i)
            for (int d = 0; d < 4; ++d)
                dp[i][d] = ndp[i][d];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int row = 0; row < 16; ++row)
        for (int d = 0; d < 4; ++d)
            result += dp[row][d];

    return result;
}
