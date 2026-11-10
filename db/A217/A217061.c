/*
 * A217061  Expansion of e.g.f. exp(A006351(x)).
 * https://oeis.org/A217061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   e.g.f. B(x) = exp(A(x)) with A = A006351 e.g.f., so a(n) =
 *            Sum_{k=1..n} binomial(n-1,k-1)*A006351(k)*a(n-k), a(0) = 1;
 *            A006351: c(1)=1, c(m+1) = 2*c(m) + Sum_{k=1..m-1}
 *            binomial(m,k)*c(k)*c(m+1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    size_t N = (size_t)n;
    seq_t *C = mem_new(seq_t, N + 2);
    seq_t *B = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    for (size_t i = 0; i < N + 2; i++) { C[i] = 0; B[i] = 0; row[i] = 0; }
    C[1] = 1;
    row[0] = 1;
    for (size_t m = 1; m < N; m++) {
        for (size_t k = m; k >= 1; k--) row[k] += row[k - 1];
        seq_t s = C[m] * 2;
        for (size_t k = 1; k + 1 <= m; k++) {
            seq_t t = row[k] * C[k];
            t *= C[m + 1 - k];
            s += t;
        }
        C[m + 1] = s;
    }
    for (size_t i = 0; i < N + 2; i++) row[i] = 0;
    row[0] = 1;
    B[0] = 1;
    for (size_t m = 1; m <= N; m++) {
        if (m >= 2) {
            for (size_t k = m - 1; k >= 1; k--) row[k] += row[k - 1];
        }
        seq_t s = 0;
        for (size_t k = 1; k <= m; k++) {
            seq_t t = row[k - 1] * C[k];
            t *= B[m - k];
            s += t;
        }
        B[m] = s;
    }
    seq_t res = B[N];
    mem_free(C);
    mem_free(B);
    mem_free(row);
    return res;
}
