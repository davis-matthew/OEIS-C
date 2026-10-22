/*
 * A000544  One-half of the number of permutations p of [n] having exactly two
 *          runs of adjacent entries differing by 1.
 * https://oeis.org/A000544
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x^2*Sum_{k>=0} k*k!*(x^2-x+k-1)*y^k/((x^2-1)^2*(x-1)^2), y
 *            = x(1-x)/(1+x). Using H(x) = Sum_k k!*y^k = Sum b(n)x^n
 *            (A002464: b(n) = (n+1)b(n-1) - (n-2)b(n-2) - (n-5)b(n-3) +
 *            (n-3)b(n-4), b(0..4) = 1,1,0,0,2) and y^2*H' = H - 1 - y*H, the
 *            g.f. reduces to G = (P(x)*H(x) - R(x)) / ((1-x)^6*(1+x...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t bw[7];   /* bw[i] = b(t-1-i) after processing index t-1 */
    static seq_t st[8];   /* states of the 8 cascaded divisions */
    static seq_t last;
    static idx_t done = 0; /* number of indices processed */
    static const int binit[5] = {1, 1, 0, 0, 2};
    static const int rco[6] = {1, -1, 0, 4, 1, -1};
    int i;

    if (n < 4) return 0;
    if (done > 0 && n + 1 == done) return last;
    if (done == 0 || n + 1 < done) {
        for (i = 0; i < 7; i++) bw[i] = 0;
        for (i = 0; i < 8; i++) st[i] = 0;
        last = 0;
        done = 0;
    }
    while (done <= n) {
        idx_t t = done;
        seq_t b, e, v;
        if (t <= 4) {
            b = (seq_t)binit[t];
        } else {
            b = (seq_t)(t + 1) * bw[0] + (seq_t)(t - 3) * bw[3]
              - (seq_t)(t - 2) * bw[1] - (seq_t)(t - 5) * bw[2];
        }
        for (i = 6; i > 0; i--) bw[i] = bw[i - 1];
        bw[0] = b;
        e = bw[0] + 2 * bw[2] + 2 * bw[3] + 2 * bw[5] - 2 * bw[1] - bw[6];
        if (t < 6) {
            int r = rco[t];
            if (r > 0) e -= (seq_t)r;
            else if (r < 0) e += (seq_t)(-r);
        }
        v = e;
        for (i = 0; i < 6; i++) {      /* divide by (1-x) six times */
            st[i] += v;
            v = st[i];
        }
        for (i = 6; i < 8; i++) {      /* divide by (1+x) twice */
            st[i] = v - st[i];
            v = st[i];
        }
        last = v;
        done++;
    }
    return last;
}
