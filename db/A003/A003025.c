/*
 * A003025  Number of n-node labeled acyclic digraphs with 1 out-point.
 * https://oeis.org/A003025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * k * C(n,k) * 2^(k*(n-k)) *
 *            A003024(n-k), where A003024(0) = 1 and A003024(m) = Sum_{k=1..m}
 *            (-1)^(k-1) * C(m,k) * 2^(k*(m-k)) * A003024(m-k) (labeled DAGs
 *            counted by inclusion-exclusion on the set of sinks)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   a001192_zalloc, a003025_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001192_zalloc.h"
#include "../utils/a003025_p2.h"

seq_t a(idx_t n)
{
    seq_t *A = a001192_zalloc(n + 1);
    seq_t *C = a001192_zalloc(n + 1);
    seq_t pos, neg, t, res;
    idx_t m, k;

    A[0] = 1;
    C[0] = 1;
    for (m = 1; m <= n; m++) {
        /* update C to row m of Pascal's triangle */
        for (k = m; k >= 1; k--)
            C[k] = C[k] + C[k - 1];
        if (m == n) break;
        pos = 0;
        neg = 0;
        for (k = 1; k <= m; k++) {
            t = C[k] * a003025_p2(k * (m - k)) * A[m - k];
            if (k & 1) pos += t; else neg += t;
        }
        A[m] = pos - neg;
    }

    pos = 0;
    neg = 0;
    for (k = 1; k <= n; k++) {
        t = (seq_t)k * C[k] * a003025_p2(k * (n - k)) * A[n - k];
        if (k & 1) pos += t; else neg += t;
    }
    res = pos - neg;
    free(A);
    free(C);
    return res;
}
