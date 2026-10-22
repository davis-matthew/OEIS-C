/*
 * A002755  Number of bipartite partitions of n white objects and 6 black
 *          ones.
 * https://oeis.org/A002755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. prod_{(i,j)!=(0,0)} 1/(1-x^i*y^j); a(n) = [x^n y^6]. Over
 *            all partitions of 6 with k_j parts equal to j: a(n) = Sum_{k}
 *            [x^n] P(x) * Prod_j 1/((1-x)(1-x^2)...(1-x^{k_j})), where P(x) =
 *            Prod_{m>=1} 1/(1-x^m) (A000041, via pentagonal recurrence)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 63, u64 n <= 257,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *Pt = 0;
static idx_t Plen = 0;

static void ensure_P(idx_t n) {
    if (n < Plen) return;
    idx_t newlen = n + 1;
    if (newlen < 2 * Plen) newlen = 2 * Plen;
    seq_t *np = (seq_t *)calloc((size_t)newlen, sizeof(seq_t));
    for (idx_t i = 0; i < Plen; i++) np[i] = Pt[i];
    if (Pt) free(Pt);
    Pt = np;
    for (idx_t m = Plen; m < newlen; m++) {
        if (m == 0) { Pt[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += Pt[m - g1];
                if (g2 <= m) pos += Pt[m - g2];
            } else {
                neg += Pt[m - g1];
                if (g2 <= m) neg += Pt[m - g2];
            }
        }
        Pt[m] = pos - neg;
    }
    Plen = newlen;
}

static int cnt[7];

static void rec(int j, int rem, idx_t n, seq_t *buf, seq_t *total) {
    if (j == 0) {
        if (rem != 0) return;
        for (idx_t i = 0; i <= n; i++) buf[i] = Pt[i];
        for (int jj = 1; jj <= 6; jj++) {
            for (int m = 1; m <= cnt[jj]; m++) {
                for (idx_t i = (idx_t)m; i <= n; i++) buf[i] += buf[i - m];
            }
        }
        *total += buf[n];
        return;
    }
    for (int c = 0; c * j <= rem; c++) {
        cnt[j] = c;
        rec(j - 1, rem - c * j, n, buf, total);
    }
    cnt[j] = 0;
}

seq_t a(idx_t n) {
    ensure_P(n);
    seq_t *buf = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t total = 0;
    for (int j = 0; j <= 6; j++) cnt[j] = 0;
    rec(6, 6, n, buf, &total);
    free(buf);
    return total;
}
