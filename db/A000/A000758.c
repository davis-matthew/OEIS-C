/*
 * A000758  Sum over rooted plane trees with n edges of (height - 1); height =
 *          measured edges.
 * https://oeis.org/A000758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{h=2..n} #{Dyck paths of semilength n with height >=
 *            h}, where #{height >= h} = Sum_{m>=1} [b(n+m(h+1)-1) -
 *            2*b(n+m(h+1)) + b(n+m(h+1)+1)], b(j) = binomial(2n, j) (de
 *            Bruijn-Knuth-Rice reflection). Grouping by d = m(h+1): a(n) =
 *            Sum_{d=3..n+1} c(d) * (b(n+d-1) - 2*b(n+d) + b(n+d+1)), c...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;
    idx_t N2 = 2 * n;
    int *c = (int *)calloc((size_t)(n + 2), sizeof(int));
    for (idx_t p = 3; p <= n + 1; p++)
        for (idx_t d = p; d <= n + 1; d += p)
            c[d]++;

    seq_t pos = 0, neg = 0;
    if (n <= 150) {
        seq_t row[302];
        idx_t i, r, k;
        row[0] = 1;
        for (i = 1; i <= N2; i++) row[i] = 0;
        for (r = 1; r <= N2; r++)
            for (k = r; k >= 1; k--)
                row[k] += row[k - 1];
        for (idx_t d = 3; d <= n + 1; d++) {
            idx_t j = n + d;
            seq_t bm = row[j - 1];
            seq_t bj = 0, bp = 0;
            if (j <= N2) bj = row[j];
            if (j + 1 <= N2) bp = row[j + 1];
            seq_t cc = (seq_t)c[d];
            pos += cc * (bm + bp);
            neg += cc * 2 * bj;
        }
    } else {
        /* b(n+2) = binomial(2n, n-2), computed multiplicatively */
        seq_t bm = 1;
        for (idx_t i = 1; i <= n - 2; i++) {
            bm *= (seq_t)(n + 2 + i);
            bm /= (seq_t)i;
        }
        seq_t bj = bm * (seq_t)(n - 2) / (seq_t)(n + 3);
        seq_t bp = bj * (seq_t)(n - 3) / (seq_t)(n + 4);
        for (idx_t d = 3; d <= n + 1; d++) {
            idx_t j = n + d;
            seq_t cc = (seq_t)c[d];
            pos += cc * (bm + bp);
            neg += cc * 2 * bj;
            bm = bj;
            bj = bp;
            if (j + 1 < N2)
                bp = bp * (seq_t)(N2 - (j + 1)) / (seq_t)(j + 2);
            else
                bp = 0;
        }
    }
    free(c);
    return pos - neg;
}
