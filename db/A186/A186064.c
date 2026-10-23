/*
 * A186064  Number of (n+2)X3 0..2 arrays with each 3X3 subblock having a
 *          positive determinant.
 * https://oeis.org/A186064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A186064: Number of (n+2)×3 arrays with entries 0,1,2 where every
   3×3 subblock has a positive determinant. */

#include <stdint.h>
#include <stdbool.h>

#define R     27              /* number of possible 3‑row patterns 3³ */
#define PAIR  (R*R)           /* number of ordered pairs of rows */

/* determinant of a 3×3 matrix given as three rows of length 3 */
static int det3(const int *r0, const int *r1, const int *r2)
{
    int a00 = r0[0], a01 = r0[1], a02 = r0[2];
    int a10 = r1[0], a11 = r1[1], a12 = r1[2];
    int a20 = r2[0], a21 = r2[1], a22 = r2[2];
    return a00 * (a11 * a22 - a12 * a21)
         - a01 * (a10 * a22 - a12 * a20)
         + a02 * (a10 * a21 - a11 * a20);
}

seq_t a(idx_t n)
{
    /* Pre‑compute all rows and allowed transitions once. */
    static int  rows[R][3];
    static unsigned char allowedCnt[PAIR];
    static unsigned char allowedList[PAIR][R];
    static bool initialized = false;
    if (!initialized) {
        /* list all 27 3-row patterns */
        for (int i = 0; i < R; ++i) {
            rows[i][0] = i / 9;
            rows[i][1] = (i / 3) % 3;
            rows[i][2] = i % 3;
        }

        /* for each ordered pair of rows, store third rows that yield
           a positive determinant for the 3×3 subblock. */
        for (int p = 0; p < R; ++p) {
            for (int q = 0; q < R; ++q) {
                int idx = p * R + q;
                unsigned char cnt = 0;
                for (int r = 0; r < R; ++r) {
                    if (det3(rows[p], rows[q], rows[r]) > 0) {
                        allowedList[idx][cnt++] = (unsigned char)r;
                    }
                }
                allowedCnt[idx] = cnt;
            }
        }
        initialized = true;
    }

    /* DP state: dp[prev*R + curr] counts sequences ending with
       rows prev, curr. */
    static seq_t dp[PAIR];
    static seq_t newdp[PAIR];

    /* Initialise for sequences of length 2 (any pair of rows). */
    for (int i = 0; i < PAIR; ++i) {
        dp[i] = (seq_t)1;
    }

    /* Number of rows in the array is n + 2. After the initial pair,
       we need to add n more rows. */
    idx_t steps = n;
    for (idx_t step = 0; step < steps; ++step) {
        /* zero newdp */
        for (int i = 0; i < PAIR; ++i) {
            newdp[i] = (seq_t)0;
        }

        for (int idx = 0; idx < PAIR; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            unsigned char cnt = allowedCnt[idx];
            for (unsigned char k = 0; k < cnt; ++k) {
                unsigned char nxt = allowedList[idx][k];
                int nextIdx = ((idx % R) * R) + nxt;
                newdp[nextIdx] += val;
            }
        }

        /* swap dp and newdp */
        for (int i = 0; i < PAIR; ++i) {
            dp[i] = newdp[i];
        }
    }

    /* Sum all sequences of length n+2. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < PAIR; ++i) {
        total += dp[i];
    }
    return total;
}
