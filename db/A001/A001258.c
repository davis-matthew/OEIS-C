/*
 * A001258  Number of labeled n-node trees with unlabeled end-points.
 * https://oeis.org/A001258
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + Sum_{m=2..n-2} Sum_{i=1..m} (-1)^(m-i) *
 *            binomial(m,i) * i^(m-2) * binomial(n-m+i-1, i-1) (m = number of
 *            labeled internal nodes; the inner sum counts labeled trees on m
 *            nodes carrying n-m unlabeled leaves, every leaf of the inner
 *            tree getting at least one)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n <= 3) return 1;
    idx_t L = n + 1;
    seq_t *cn = alloc_zero(L);  /* row n-1 of Pascal's triangle */
    seq_t *cm = alloc_zero(L);  /* row m of Pascal's triangle */
    seq_t *pw = alloc_zero(L);  /* pw[r] = r^(m-2) */
    seq_t *D = alloc_zero(L);   /* D[r] = binomial(n-m+r-1, r-1) */
    idx_t i, j, r, m;

    cn[0] = 1;
    for (i = 1; i <= n - 1; i++)
        for (j = i; j >= 1; j--)
            cn[j] += cn[j - 1];

    cm[0] = 1; cm[1] = 2; cm[2] = 1;
    for (r = 0; r <= n; r++) pw[r] = 1;
    D[0] = 0; D[1] = 1; D[2] = cn[1];

    seq_t pos = 1, neg = 0;
    for (m = 2; m + 2 <= n; m++) {
        if (m > 2) {
            for (j = m; j >= 1; j--) cm[j] += cm[j - 1];
            for (r = 0; r <= m; r++) pw[r] *= (seq_t)r;
            pw[m] = 1;
            for (r = 0; r + 2 < m; r++) pw[m] *= (seq_t)m;
            /* binomial(N-1+r-1, r-1) = binomial(N+r-1, r-1) - binomial(N+r-2, r-2), using old values */
            for (r = m - 1; r >= 2; r--) D[r] -= D[r - 1];
            D[m] = cn[m - 1];
        }
        for (r = 1; r <= m; r++) {
            seq_t t = cm[r] * pw[r] * D[r];
            if ((m - r) & 1) neg += t;
            else pos += t;
        }
    }

    release(cn, L);
    release(cm, L);
    release(pw, L);
    release(D, L);
    return pos - neg;
}
