/*
 * A052877  E.g.f.: exp(x)-1+log(-1/(-2+exp(x))).
 * https://oeis.org/A052877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Σ_{k=1}∞ k^{n-1} / 2^k (a(0)=0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    /* Allocate arrays for terms up to n and powers of two up to n-1 */
    size_t N = (size_t)n;
    seq_t *A = mem_new(seq_t, (N + 1));
    seq_t *pow2 = mem_new(seq_t, N);
    if (!A || !pow2) {
        /* allocation failed; return 0 as a safe fallback */
        mem_free(A);
        mem_free(pow2);
        return 0;
    }

    A[0] = 0;
    A[1] = 2;
    pow2[0] = 1;
    for (size_t i = 1; i < N; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    for (size_t i = 2; i <= N; ++i) {
        seq_t sum = 0;
        for (size_t j = 1; j < i; ++j) {
            seq_t coeff = binomial((idx_t)(i - 1), (idx_t)(j - 1));
            sum += coeff * A[j];
        }
        sum -= pow2[i - 1];
        sum += (seq_t)3;
        A[i] = sum;
    }

    seq_t result = A[N];
    mem_free(A);
    mem_free(pow2);
    return result;
}
