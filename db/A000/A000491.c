/*
 * A000491  Number of bipartite partitions of n white objects and 5 black
 *          ones.
 * https://oeis.org/A000491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n y^5] Prod_{(i,j)!=(0,0)} 1/(1 - x^i y^j) = Sum over
 *            partitions 1^m1 2^m2 ... 5^m5 of 5 of [x^n] P(x) * Prod_j
 *            1/((1-x)(1-x^2)...(1-x^mj)), where P(x) = Prod_{k>=1} 1/(1-x^k)
 *            (partition numbers, via Euler's pentagonal recurrence)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 68, u64 n <= 273,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *part_tab = NULL;
static idx_t part_len = 0;

static void ensure_parts(idx_t n) {
    idx_t need = n + 1, m, k;
    seq_t *t;
    if (need <= part_len) return;
    t = (seq_t *)calloc((size_t)need, sizeof(seq_t));
    for (m = 0; m < part_len; m++) t[m] = part_tab[m];
    for (m = part_len; m < need; m++) {
        seq_t pos = 0, neg = 0;
        if (m == 0) { t[0] = 1; continue; }
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2, g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += t[m - g1];
                if (g2 <= m) pos += t[m - g2];
            } else {
                neg += t[m - g1];
                if (g2 <= m) neg += t[m - g2];
            }
        }
        t[m] = pos - neg;
    }
    part_tab = t;
    part_len = need;
}

/* nonzero multiplicities of each partition of 5 */
static const int mult_lists[7][3] = {
    {1, 0, 0},  /* 5 */
    {1, 1, 0},  /* 4+1 */
    {1, 1, 0},  /* 3+2 */
    {1, 2, 0},  /* 3+1+1 */
    {2, 1, 0},  /* 2+2+1 */
    {1, 3, 0},  /* 2+1+1+1 */
    {5, 0, 0}   /* 1+1+1+1+1 */
};

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t *f;
    int p, q;
    idx_t i, k;
    ensure_parts(n);
    f = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    for (p = 0; p < 7; p++) {
        for (i = 0; i <= n; i++) f[i] = part_tab[i];
        for (q = 0; q < 3; q++) {
            int m = mult_lists[p][q];
            for (k = 1; k <= (idx_t)m; k++)
                for (i = k; i <= n; i++) f[i] += f[i - k];
        }
        total += f[n];
    }
    free(f);
    return total;
}
