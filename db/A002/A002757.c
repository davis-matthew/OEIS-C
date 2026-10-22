/*
 * A002757  Number of bipartite partitions of n white objects and 8 black
 *          ones.
 * https://oeis.org/A002757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. for column 8: [y^8] Product_{(i,j)!=(0,0)} 1/(1-x^i*y^j) =
 *            P(x) * Sum_{lambda partition of 8} Product_j
 *            1/(x;x)_{m_j(lambda)}, where P(x) = Product_{i>=1} 1/(1-x^i)
 *            (computed by the pentagonal recurrence) and m_j(lambda) =
 *            multiplicity of part j; a(n) = Sum_{k=0..n} G(k)*p(n-k)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 54, u64 n <= 230, u128 n <= 984,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *Pa = 0, *Ga = 0, *Ta = 0;
static idx_t cap = 0;

static void add_lambda(const int *cnt, idx_t N) {
    idx_t k;
    int j, i;
    for (k = 0; k < N; k++) Ta[k] = 0;
    Ta[0] = 1;
    for (j = 1; j <= 8; j++)
        for (i = 1; i <= cnt[j]; i++)
            for (k = (idx_t)i; k < N; k++) Ta[k] += Ta[k - i];
    for (k = 0; k < N; k++) Ga[k] += Ta[k];
}

static void gen(int rem, int maxp, int *cnt, idx_t N) {
    int p;
    if (rem == 0) { add_lambda(cnt, N); return; }
    for (p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        cnt[p]++;
        gen(rem - p, p, cnt, N);
        cnt[p]--;
    }
}

static void build(idx_t N) {
    idx_t m, k;
    int cnt[9];
    int i;
    Pa = alloc_zero(N);
    Ga = alloc_zero(N);
    Ta = alloc_zero(N);
    Pa[0] = 1;
    for (m = 1; m < N; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2, g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) pos += Pa[m - g1]; else neg += Pa[m - g1];
            if (g2 <= m) {
                if (k & 1) pos += Pa[m - g2]; else neg += Pa[m - g2];
            }
        }
        Pa[m] = pos - neg;
    }
    for (i = 0; i <= 8; i++) cnt[i] = 0;
    for (k = 0; k < N; k++) Ga[k] = 0;
    gen(8, 8, cnt, N);
    cap = N;
}

seq_t a(idx_t n) {
    idx_t k;
    seq_t s = 0;
    if (n >= cap) {
        idx_t nc = 2 * cap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        build(nc);
    }
    for (k = 0; k <= n; k++) s += Ga[k] * Pa[n - k];
    return s;
}
