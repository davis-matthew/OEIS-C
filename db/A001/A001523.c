/*
 * A001523  Number of stacks, or planar partitions of n; also number of weakly
 *          unimodal compositions of n.
 * https://oeis.org/A001523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: 1 + (Sum_{k>0} -(-1)^k x^(k(k+1)/2)) / (Product_{k>0}
 *            (1-x^k))^2, i.e. a(0) = 1 and, for n > 0, a(n) = Sum_{k>=1,
 *            k(k+1)/2<=n} (-1)^(k+1) * A000712(n - k(k+1)/2), where
 *            A000712(m) = Sum_{i=0..m} p(i)*p(m-i), and p = A000041 comes
 *            from Euler's pentagonal recurrence
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 70, u64 n <= 222,
 *            u128 n <= 759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a001523_P = NULL;
static idx_t a001523_plen = 0;

static void a001523_ensure(idx_t n) {
    if (n < a001523_plen) return;
    idx_t nl = n + 1;
    if (nl < 2 * a001523_plen) nl = 2 * a001523_plen;
    seq_t *q = (seq_t *)calloc((size_t)nl, sizeof(seq_t));
    for (idx_t i = 0; i < a001523_plen; i++) q[i] = a001523_P[i];
    free(a001523_P);
    a001523_P = q;
    for (idx_t m = a001523_plen; m < nl; m++) {
        if (m == 0) { q[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            seq_t t = q[m - g1];
            if (g2 <= m) t += q[m - g2];
            if (k & 1) pos += t; else neg += t;
        }
        q[m] = pos - neg;
    }
    a001523_plen = nl;
}

static seq_t a001523_p2(idx_t m) {
    const seq_t *P = a001523_P;
    seq_t s = 0;
    idx_t i = 0;
    for (; 2 * i < m; i++) s += P[i] * P[m - i];
    s += s;
    if (2 * i == m) s += P[i] * P[i];
    return s;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    a001523_ensure(n);
    seq_t pos = 0, neg = 0;
    for (idx_t k = 1;; k++) {
        idx_t T = k * (k + 1) / 2;
        if (T > n) break;
        seq_t v = a001523_p2(n - T);
        if (k & 1) pos += v; else neg += v;
    }
    return pos - neg;
}
