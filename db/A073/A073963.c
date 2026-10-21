/*
 * A073963  Number of strings of length n over Z_5 with trace 0 and subtrace
 *          0.
 * https://oeis.org/A073963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n; t, s) = a(n-1; t, s) + a(n-1; t+4, s+4*t+1) + a(n-1; t+3,
 *            s+3*t+4) + a(n-1; t+2, s+2*t+4) + a(n-1; t+1, s+t+1) where all
 *            arithmetic on the right is modulo 5. Initial state: a(0; 0,0) =
 *            1, all other a(0; t,s) = 0. The required sequence is a(n) = a(n;
 *            0,0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* dpPrev[t][s] holds a(k; t, s) for current k;
       dpCur will store the next step.
       Indices 0..4 represent elements of Z_5. */
    seq_t dpPrev[5][5] = {0};
    dpPrev[0][0] = 1;          /* a(0;0,0) = 1 */
    seq_t dpCur[5][5];

    for (idx_t step = 1; step <= n; ++step) {
        for (int t = 0; t < 5; ++t) {
            for (int s = 0; s < 5; ++s) {
                seq_t val = dpPrev[t][s];

                int t4 = (t + 4) % 5;
                int s4 = (s + 4 * t + 1) % 5;
                val += dpPrev[t4][s4];

                int t3 = (t + 3) % 5;
                int s3 = (s + 3 * t + 4) % 5;
                val += dpPrev[t3][s3];

                int t2 = (t + 2) % 5;
                int s2 = (s + 2 * t + 4) % 5;
                val += dpPrev[t2][s2];

                int t1 = (t + 1) % 5;
                int s1 = (s + t + 1) % 5;
                val += dpPrev[t1][s1];

                dpCur[t][s] = val;
            }
        }
        /* copy dpCur into dpPrev for next iteration */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                dpPrev[t][s] = dpCur[t][s];
    }
    return dpPrev[0][0];      /* a(n;0,0) */
}
