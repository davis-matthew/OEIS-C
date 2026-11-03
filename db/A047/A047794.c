/*
 * A047794  a(n) = Sum_{k=0..n} C(n,k)*|Stirling1(n,k)*Stirling2(n,k)|.
 * https://oeis.org/A047794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) *
 *            |Stirling1(n,k)*Stirling2(n,k)|
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 270 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047794: a(n) = Sum_{k=0..n} C(n,k) |s1(n,k)| S2(n,k). The rows of the
 * unsigned Stirling numbers of both kinds and of Pascal's triangle are built
 * up to n by their recurrences; ring operations only, so exact modulo the
 * width (the original divided a binomial after it had wrapped, and its C and
 * C++ paths differed). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *c = mem_new(seq_t, N + 2), *S = mem_new(seq_t, N + 2), *B = mem_new(seq_t, N + 2);
    if (!c || !S || !B) abort();
    c[0] = S[0] = B[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) {
            c[k] = c[k - 1] + (seq_t)(m - 1) * c[k];
            S[k] = S[k - 1] + (seq_t)k * S[k];
            B[k] = B[k] + B[k - 1];
        }
        c[0] = S[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) sum += B[k] * c[k] * S[k];
    mem_free(c);
    mem_free(S);
    mem_free(B);
    return sum;
}
