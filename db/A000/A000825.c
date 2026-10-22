/*
 * A000825  Expansion of cos x (1 + sin x ) /cos 2x.
 * https://oeis.org/A000825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   e.g.f. cos(x)(1+sin(x))/cos(2x) = cos(x)/cos(2x) + tan(2x)/2,
 *            and cos(x)/cos(2x) = (1/(cos x - sin x) + 1/(cos x + sin x))/2.
 *            With sec^(n)(y) = sec(y)*P_n(tan y), P_0 = 1, P_{n+1}(t) =
 *            t*P_n(t) + (1+t^2)*P_n'(t): a(2k) = P_{2k}(1). With tan^(n)(y) =
 *            Q_n(tan y), Q_0 = t, Q_{n+1}(t) = (1+t^2)*Q_n'(...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *vals = NULL;
static idx_t have = 0;

static void build(idx_t N) {
    idx_t len = N + 3, m, j;
    seq_t *p = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *np = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *q = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *nq = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *nv = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t pow4 = 1;
    p[0] = 1;
    q[1] = 1;
    for (m = 0; m <= N; m++) {
        if (m % 2 == 0) {
            seq_t s = 0;
            for (j = 0; j <= m; j += 2) s += p[j];
            nv[m] = s;
        } else {
            nv[m] = pow4 * q[0];
            pow4 *= 4;
        }
        if (m == N) break;
        for (j = (m + 1) & 1; j <= m + 1; j += 2) {
            seq_t v = 0;
            if (j >= 1) v += (seq_t)j * p[j - 1];
            if (j + 1 <= m) v += (seq_t)(j + 1) * p[j + 1];
            np[j] = v;
        }
        for (j = m & 1; j <= m + 2; j += 2) {
            seq_t v = 0;
            if (j + 1 <= m + 1) v += (seq_t)(j + 1) * q[j + 1];
            if (j >= 2) v += (seq_t)(j - 1) * q[j - 1];
            nq[j] = v;
        }
        { seq_t *t = p; p = np; np = t; }
        { seq_t *t = q; q = nq; nq = t; }
    }
    free(p); free(np); free(q); free(nq);
    if (vals) free(vals);
    vals = nv;
    have = N + 1;
}

seq_t a(idx_t n) {
    if (vals == NULL || n >= have) build(n + 64);
    return vals[n];
}
