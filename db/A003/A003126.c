/*
 * A003126  Value of an urn with n balls of type -1 and n+1 balls of type +1.
 * https://oeis.org/A003126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n-1, n) * binomial(2n-1, n-1), where B(m,p) is the
 *            value of Shepp's urn: V(0,p) = p, V(m,0) = 0, V(m,p) = max(0,
 *            m/(m+p)*(V(m-1,p)-1) + p/(m+p)*(V(m,p-1)+1)). This uses the
 *            integer form W(m,p) = C(m+p,m)*V(m,p): W(m,p) = max(0, W(m-1,p)
 *            + W(m,p-1) + C(m+p-1,m) - C(m+p-1,m-1)), W(0,p) = p...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    idx_t L = n + 1;
    idx_t m, p;
    seq_t res;
#ifdef __cplusplus
    std::vector<seq_t> Wv(L), Bv(L);
    seq_t *W = &Wv[0];
    seq_t *B = &Bv[0];
#else
    seq_t *W = (seq_t *)malloc(L * sizeof(seq_t));
    seq_t *B = (seq_t *)malloc(L * sizeof(seq_t));
#endif
    /* row m = 0: W(0,p) = p, B(0,p) = C(p,0) = 1 */
    for (p = 0; p < L; p++) {
        W[p] = (seq_t)p;
        B[p] = (seq_t)1;
    }
    for (m = 1; m < n; m++) {
        W[0] = (seq_t)0;
        B[0] = (seq_t)1;
        for (p = 1; p < L; p++) {
            seq_t wup = W[p];       /* W(m-1,p) */
            seq_t wleft = W[p - 1]; /* W(m,p-1) */
            seq_t bup = B[p];       /* C(m+p-1, m-1) */
            seq_t bleft = B[p - 1]; /* C(m+p-1, m) */
            int positive;
            if (bleft >= bup) {
                positive = 1;
            } else {
                seq_t d = bup - bleft;
                if (wup >= d) positive = 1;
                else positive = (wleft >= d - wup) ? 1 : 0;
            }
            if (positive) {
                W[p] = wup + wleft + bleft - bup;
            } else {
                W[p] = (seq_t)0;
            }
            B[p] = bup + bleft;
        }
    }
    res = W[n];
#ifndef __cplusplus
    free(W);
    free(B);
#endif
    return res;
}
