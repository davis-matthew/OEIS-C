/*
 * A231691  Cardinalities of the symmetric operad of dotted red and white
 *          trees.
 * https://oeis.org/A231691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = e.g.f. with f(A(x)) = x, f(y) = -y - g(-y), g = e.g.f. of
 *            A052878; equivalently A'(1-2A-3A^2-A^3) = 1+4A+4A^2+A^3, A(0)=0,
 *            A'(0)=1, solved with binomial convolutions on e.g.f.
 *            coefficients
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 271 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    size_t N = (size_t)n;
    seq_t *A  = mem_new(seq_t, N + 2);
    seq_t *P2 = mem_new(seq_t, N + 2);
    seq_t *P3 = mem_new(seq_t, N + 2);
    seq_t *Q  = mem_new(seq_t, N + 2);
    seq_t *b  = mem_new(seq_t, N + 2);
    b[0] = (seq_t)1;
    A[0] = (seq_t)0;
    for (size_t m = 0; m < N; ++m) {
        if (m > 0) {
            for (size_t j = m; j >= 1; --j) b[j] = b[j] + b[j - 1];
        }
        seq_t s2 = (seq_t)0;
        for (size_t k = 0; k <= m; ++k) s2 = s2 + b[k] * A[k] * A[m - k];
        P2[m] = s2;
        seq_t s3 = (seq_t)0;
        for (size_t k = 0; k <= m; ++k) s3 = s3 + b[k] * A[k] * P2[m - k];
        P3[m] = s3;
        Q[m] = (seq_t)2 * A[m] + (seq_t)3 * P2[m] + P3[m];
        seq_t rhs = (seq_t)4 * A[m] + (seq_t)4 * P2[m] + P3[m];
        if (m == 0) rhs = rhs + (seq_t)1;
        for (size_t k = 0; k + 1 <= m; ++k)
            rhs = rhs + b[k] * A[k + 1] * Q[m - k];
        A[m + 1] = rhs;
    }
    seq_t res = A[N];
    mem_free(A); mem_free(P2); mem_free(P3); mem_free(Q); mem_free(b);
    return res;
}
