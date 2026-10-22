/*
 * A001487  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^9 in powers of x.
 * https://oeis.org/A001487
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n-9)] h(x)^9, where h(x) = (Product_{j>=1} (1-(-x)^j)
 *            - 1)/x. By Euler's pentagonal theorem, Product_{j>=1} (1-(-x)^j)
 *            = Sum_k (-1)^k (-1)^m x^m with m = k(3k-1)/2 or k(3k+1)/2, so h
 *            is sparse. h^9 is built by 8 sparse multiplications
 * Range:     i8 n <= 14, i16 n <= 43, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A001487_CAP 32768
#define A001487_SP 1024

static seq_t a001487_bufA[A001487_CAP];
static seq_t a001487_bufB[A001487_CAP];
static seq_t *a001487_res = 0;
static idx_t a001487_lim = 0;

static void a001487_compute(idx_t L) {
    idx_t sidx[A001487_SP];
    int sval[A001487_SP];
    int ns = 0;
    for (idx_t k = 1;; k++) {
        idx_t m1 = k * (3 * k - 1) / 2;
        idx_t m2 = k * (3 * k + 1) / 2;
        int sk = (k & 1) ? -1 : 1;
        if (m1 - 1 >= L) break;
        sidx[ns] = m1 - 1;
        sval[ns] = sk * ((m1 & 1) ? -1 : 1);
        ns++;
        if (m2 - 1 < L) {
            sidx[ns] = m2 - 1;
            sval[ns] = sk * ((m2 & 1) ? -1 : 1);
            ns++;
        }
    }
    seq_t *cur = a001487_bufA;
    seq_t *nxt = a001487_bufB;
    for (idx_t i = 0; i < L; i++) cur[i] = 0;
    for (int t = 0; t < ns; t++) cur[sidx[t]] = sval[t];
    for (int p = 1; p < 9; p++) {
        for (idx_t i = 0; i < L; i++) {
            seq_t s = 0;
            for (int t = 0; t < ns && sidx[t] <= i; t++) {
                if (sval[t] > 0) s += cur[i - sidx[t]];
                else s -= cur[i - sidx[t]];
            }
            nxt[i] = s;
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }
    a001487_res = cur;
    a001487_lim = L;
}

seq_t a(idx_t n) {
    if (n < 9) return 0;
    idx_t m = n - 9;
    if (m >= A001487_CAP) return 0;
    if (m >= a001487_lim) {
        idx_t L = 2 * a001487_lim;
        if (L < m + 1) L = m + 1;
        if (L < 64) L = 64;
        if (L > A001487_CAP) L = A001487_CAP;
        a001487_compute(L);
    }
    return a001487_res[m];
}
