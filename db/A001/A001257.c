/*
 * A001257  Squares of numbers of rooted trees.
 * https://oeis.org/A001257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000081(n+1)^2, where r = A000081 satisfies r(1) = 1,
 *            r(m+1) = (1/m) * Sum_{k=1..m} (Sum_{d|k} d*r(d)) * r(m-k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *R = 0;   /* R[i] = A000081(i) */
static seq_t *C = 0;   /* C[i] = Sum_{d|i} d*R[d] */
static idx_t cnt = 0;  /* indices 1..cnt computed */
static idx_t cap = 0;

static void ensure(idx_t m) {
    if (m + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < m + 1) nc = m + 1;
        if (nc < 64) nc = 64;
        seq_t *nr = (seq_t *)malloc(nc * sizeof(seq_t));
        seq_t *ncc = (seq_t *)malloc(nc * sizeof(seq_t));
        for (idx_t i = 0; i < nc; i++) {
            nr[i] = 0;
            ncc[i] = 0;
        }
        for (idx_t i = 0; i <= cnt && i < cap; i++) {
            nr[i] = R[i];
            ncc[i] = C[i];
        }
        free(R);
        free(C);
        R = nr;
        C = ncc;
        cap = nc;
    }
    for (idx_t i = cnt + 1; i <= m; i++) {
        if (i == 1) {
            R[1] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; k++)
                sum += C[k] * R[i - k];
            R[i] = sum / (seq_t)(i - 1);
        }
        seq_t s = 0;
        for (idx_t d = 1; d * d <= i; d++) {
            if (i % d == 0) {
                idx_t e = i / d;
                s += (seq_t)d * R[d];
                if (e != d) s += (seq_t)e * R[e];
            }
        }
        C[i] = s;
        cnt = i;
    }
}

seq_t a(idx_t n) {
    idx_t m = n + 1;
    ensure(m);
    seq_t r = R[m];
    return r * r;
}
