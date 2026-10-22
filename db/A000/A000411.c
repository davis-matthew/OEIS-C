/*
 * A000411  Generalized tangent numbers d(6,n).
 * https://oeis.org/A000411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (2*n-1)! * [x^(2*n-1)] 2*sin(3*x)/(2*cos(4*x)-1). With
 *            f(x) = Sum f_k x^k/k!, the identity f(x)*(2*cos(4*x)-1) =
 *            2*sin(3*x) gives f_k = s_k - Sum_{j<k, k-j even}
 *            C(k,j)*f_j*c_{k-j}, where c_{2m} = 2*(-1)^m*16^m and s_{2m+1} =
 *            2*(-1)^m*3^(2m+1). Then a(n) = f_{2n-1}, with a(1) = 6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   a306_alloc0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_alloc0.h"

static seq_t *F = NULL;   /* F[i] = f_{2i+1} */
static idx_t cnt = 0;

static void build(idx_t N) {
    seq_t *Fn = a306_alloc0(N);
    seq_t *R = a306_alloc0(2 * N + 1);
    seq_t *P16 = a306_alloc0(N + 1);
    idx_t rowk = 0, i, t, j, m;
    seq_t p3 = 1;

    R[0] = 1;
    P16[0] = 2;
    for (m = 1; m <= N; m++) P16[m] = P16[m - 1] * 16;

    for (i = 0; i < N; i++) {
        idx_t k = 2 * i + 1;
        seq_t pos = 0, neg = 0;
        /* advance Pascal row to k */
        while (rowk < k) {
            R[rowk + 1] = 0;
            for (j = rowk + 1; j >= 1; j--) R[j] += R[j - 1];
            rowk++;
        }
        p3 *= (i == 0) ? 3 : 9;
        if (i & 1) neg += p3 * 2; else pos += p3 * 2;
        for (t = 0; t < i; t++) {
            seq_t term;
            j = 2 * t + 1;
            m = i - t;
            term = R[j] * Fn[t] * P16[m];
            if (m & 1) pos += term; else neg += term;
        }
        Fn[i] = pos - neg;
    }
    F = Fn;
    cnt = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > cnt) {
        idx_t N = cnt * 2;
        if (N < n) N = n;
        if (N < 16) N = 16;
        build(N);
    }
    return F[n - 1];
}
