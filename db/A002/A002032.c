/*
 * A002032  Number of n-colored connected graphs on n labeled nodes.
 * https://oeis.org/A002032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*A001187(n), where A001187(0) = 1 and A001187(n) =
 *            2^C(n,2) - Sum_{k=1..n-1} C(n-1,k-1)*A001187(k)*2^C(n-k,2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   a001028_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_grow.h"

seq_t a(idx_t n) {
    static seq_t *c = 0, *P = 0, *row = 0;
    static idx_t have = 0, cap = 0;
    static seq_t pw;

    if (n + 1 > cap) {
        idx_t nc = (n + 1 > 2 * cap) ? n + 1 : 2 * cap;
        c = a001028_grow(c, cap, nc);
        P = a001028_grow(P, cap, nc);
        row = a001028_grow(row, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        c[0] = 1;
        P[0] = 1;
        row[0] = 1;   /* row[j] = C(have-1, j) */
        pw = 1;       /* pw = 2^(have-1) */
        have = 1;
    }
    while (have <= n) {
        idx_t i = have;
        idx_t k, j;
        seq_t s = 0;
        P[i] = P[i - 1] * pw;   /* 2^C(i,2) */
        pw = pw * 2;
        for (k = 1; k < i; k++)
            s += row[k - 1] * c[k] * P[i - k];
        c[i] = P[i] - s;
        row[i] = 0;
        for (j = i; j >= 1; j--)
            row[j] += row[j - 1];
        have++;
    }

    seq_t r = c[n];
    for (idx_t i = 2; i <= n; i++)
        r = r * (seq_t)i;
    return r;
}
