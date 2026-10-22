/*
 * A002763  Number of bipartite partitions.
 * https://oeis.org/A002763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (4 - x - 3*x^2 + x^3) / ((1-x)^3 * (1+x)) * Product_{k>=1}
 *            1/(1-x^k). With Q(x) = P(x) / ((1-x)^2 (1-x^2)), where P(x) is
 *            the partition g.f., a(n) = 4*Q(n) - Q(n-1) - 3*Q(n-2) + Q(n-3)
 *            (Q(k) = 0 for k < 0). p(n) uses Euler's pentagonal recurrence
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 78, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *g_p = NULL;
static seq_t *g_q = NULL;
static idx_t g_len = 0;

static void build(idx_t len) {
    seq_t *p = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *s = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *q = (seq_t *)malloc(len * sizeof(seq_t));
    idx_t m, k;
    for (m = 0; m < len; m++) { p[m] = 0; s[m] = 0; q[m] = 0; }
    p[0] = 1;
    for (m = 1; m < len; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += p[m - g1];
                if (g2 <= m) pos += p[m - g2];
            } else {
                neg += p[m - g1];
                if (g2 <= m) neg += p[m - g2];
            }
        }
        p[m] = pos - neg;
    }
    /* first cumulative sum */
    {
        seq_t acc = 0;
        for (m = 0; m < len; m++) { acc += p[m]; s[m] = acc; }
    }
    /* second cumulative sum */
    {
        seq_t acc = 0;
        for (m = 0; m < len; m++) { acc += s[m]; s[m] = acc; }
    }
    /* divide by (1 - x^2) */
    for (m = 0; m < len; m++) {
        q[m] = s[m];
        if (m >= 2) q[m] += q[m - 2];
    }
    free(s);
    if (g_p) free(g_p);
    if (g_q) free(g_q);
    g_p = p;
    g_q = q;
    g_len = len;
}

static seq_t Qv(idx_t n, idx_t sub) {
    if (sub > n) return (seq_t)0;
    return g_q[n - sub];
}

seq_t a(idx_t n) {
    if (n >= g_len) {
        idx_t len = g_len ? g_len : 64;
        while (len <= n) len *= 2;
        build(len);
    }
    {
        seq_t plus = (seq_t)4 * Qv(n, 0) + Qv(n, 3);
        seq_t minus = Qv(n, 1) + (seq_t)3 * Qv(n, 2);
        return plus - minus;
    }
}
