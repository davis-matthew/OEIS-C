/*
 * A237311  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..3 such that for
 *            every 2×2 subblock the value max + lower median – min is the
 *            same for all subblocks. For any offset 1, i.e. a(1) = 7398, a(2)
 *            = 120650, …
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The function below works with any integer type
   accepted as seq_t (8, 16, 32, 64 or 128 bit unsigned)
   or an arbitrary‑precision integer class when the file
   is compiled as C++17.  All arithmetic uses the operators
   provided by seq_t.  */
seq_t a(idx_t n)
{
    /* 256 possible 4‑tuples over {0,1,2,3} */
    static unsigned char digit[256][4];
    /* adjacency lists: edgesTo[k][r][i] = next row id,
       edgesCount[k][r] = number of successors.    */
    static unsigned char edgesTo[7][256][256];
    static unsigned char edgesCount[7][256] = {0};
    static bool init = false;

    if (!init) {
        init = true;
        /* initialise digits (base‑4 representation) */
        for (int r = 0; r < 256; ++r) {
            unsigned char d = (unsigned char) r;
            digit[r][0] = d & 3;
            digit[r][1] = (d >> 2) & 3;
            digit[r][2] = (d >> 4) & 3;
            digit[r][3] = (d >> 6) & 3;
        }

        /* helper to compute the value S for a 2×2 block
           beginning at column j (0..2).               */
        /* s[row1][row2] = max + lower‑median – min  */
        for (int r1 = 0; r1 < 256; ++r1) {
            unsigned char *a = digit[r1];
            for (int r2 = 0; r2 < 256; ++r2) {
                unsigned char *b = digit[r2];
                int s0, s1, s2, val, tmp;

                /* column 0 */
                unsigned char v0[4] = {a[0], a[1], b[0], b[1]};
                /* sorting network for 4 elements */
                if (v0[0] > v0[1]) { tmp = v0[0]; v0[0] = v0[1]; v0[1] = tmp; }
                if (v0[2] > v0[3]) { tmp = v0[2]; v0[2] = v0[3]; v0[3] = tmp; }
                if (v0[0] > v0[2]) { tmp = v0[0]; v0[0] = v0[2]; v0[2] = tmp; }
                if (v0[1] > v0[3]) { tmp = v0[1]; v0[1] = v0[3]; v0[3] = tmp; }
                if (v0[1] > v0[2]) { tmp = v0[1]; v0[1] = v0[2]; v0[2] = tmp; }
                s0 = v0[3] + v0[1] - v0[0];

                /* column 1 */
                unsigned char v1[4] = {a[1], a[2], b[1], b[2]};
                if (v1[0] > v1[1]) { tmp = v1[0]; v1[0] = v1[1]; v1[1] = tmp; }
                if (v1[2] > v1[3]) { tmp = v1[2]; v1[2] = v1[3]; v1[3] = tmp; }
                if (v1[0] > v1[2]) { tmp = v1[0]; v1[0] = v1[2]; v1[2] = tmp; }
                if (v1[1] > v1[3]) { tmp = v1[1]; v1[1] = v1[3]; v1[3] = tmp; }
                if (v1[1] > v1[2]) { tmp = v1[1]; v1[1] = v1[2]; v1[2] = tmp; }
                s1 = v1[3] + v1[1] - v1[0];

                /* column 2 */
                unsigned char v2[4] = {a[2], a[3], b[2], b[3]};
                if (v2[0] > v2[1]) { tmp = v2[0]; v2[0] = v2[1]; v2[1] = tmp; }
                if (v2[2] > v2[3]) { tmp = v2[2]; v2[2] = v2[3]; v2[3] = tmp; }
                if (v2[0] > v2[2]) { tmp = v2[0]; v2[0] = v2[2]; v2[2] = tmp; }
                if (v2[1] > v2[3]) { tmp = v2[1]; v2[1] = v2[3]; v2[3] = tmp; }
                if (v2[1] > v2[2]) { tmp = v2[1]; v2[1] = v2[2]; v2[2] = tmp; }
                s2 = v2[3] + v2[1] - v2[0];

                if (s0 == s1 && s1 == s2) {
                    unsigned char k = (unsigned char)s0;   /* 0..6 */
                    unsigned char c = edgesCount[k][r1];
                    edgesTo[k][r1][c] = (unsigned char)r2;
                    edgesCount[k][r1] = c + 1;
                }
            }
        }
    }

    /* 256*7 DP tables each of seq_t */
    seq_t dp[256];
    seq_t ndp[256];
    seq_t total = 0;

    for (unsigned char k = 0; k < 7; ++k) {
        /* initial vector: all rows are allowed as the first row */
        for (int r = 0; r < 256; ++r)
            dp[r] = 1;

        for (idx_t step = 0; step < n; ++step) {
            /* clear ndp */
            for (int r = 0; r < 256; ++r)
                ndp[r] = 0;

            for (int r = 0; r < 256; ++r) {
                seq_t val = dp[r];
                if (val == 0)
                    continue;
                unsigned char cnt = edgesCount[k][r];
                for (unsigned char e = 0; e < cnt; ++e) {
                    int nxt = edgesTo[k][r][e];
                    ndp[nxt] += val;
                }
            }

            /* swap dp and ndp */
            for (int r = 0; r < 256; ++r)
                dp[r] = ndp[r];
        }

        /* add contribution of this k */
        seq_t sum = 0;
        for (int r = 0; r < 256; ++r)
            sum += dp[r];
        total += sum;
    }
    return total;
}
