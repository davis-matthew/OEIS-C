/*
 * A058381  Number of series-parallel networks with n labeled edges, multiple
 *          edges not allowed.
 * https://oeis.org/A058381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=1..n} Stirling1(n,k)*A006351(k), n > 0 (signed
 *            Stirling numbers of the first kind); a(0) = 0. A006351: b(1) =
 *            1, b(m+1) = 2*b(m) + Sum_{k=1..m-1} binomial(m,k)*b(k)*b(m+1-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t N = (size_t)n;
    seq_t *v = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    seq_t *c = mem_new(seq_t, N + 2);
    v[1] = 1;
    row[0] = 1;
    for (size_t m = 1; m < N; m++) {
        /* advance Pascal row to row m */
        for (size_t k = m; k >= 1; k--) row[k] += row[k - 1];
        seq_t s = v[m] * 2;
        for (size_t k = 1; k + 1 <= m; k++) {
            seq_t t = row[k] * v[k];
            t *= v[m + 1 - k];
            s += t;
        }
        v[m + 1] = s;
    }
    /* unsigned Stirling numbers of the first kind, row n */
    c[0] = 1;
    for (size_t i = 1; i <= N; i++) {
        for (size_t k = i; k >= 1; k--) {
            c[k] = c[k - 1] + (seq_t)(i - 1) * c[k];
        }
        c[0] = 0;
    }
    seq_t pos = 0, neg = 0;
    for (size_t k = 1; k <= N; k++) {
        seq_t t = c[k] * v[k];
        if (((N - k) & 1) == 0) pos += t; else neg += t;
    }
    seq_t r = pos - neg;
    mem_free(v);
    mem_free(row);
    mem_free(c);
    return r;
}
