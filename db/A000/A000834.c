/*
 * A000834  Expansion of e.g.f. exp(x)*(1 + tan(x))/(1 - tan(x)).
 * https://oeis.org/A000834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. exp(x)*(sec(2x)+tan(2x)), so a(n) = Sum_{i=0..n}
 *            binomial(n,i)*2^i*A000111(i), where A000111(k) = E(k,k) from the
 *            Entringer triangle E(0,0)=1, E(k,0)=0 (k>0), E(k,j) = E(k,j-1) +
 *            E(k-1,k-j)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    seq_t *E = alloc_zero(n + 1);
    seq_t *A = alloc_zero(n + 1);
    seq_t *B = alloc_zero(n + 1);
    seq_t *C = alloc_zero(n + 1);
    seq_t *t;
    seq_t res = 0, p = 1;
    idx_t i, j, k;

    A[0] = 1;
    E[0] = 1;
    for (k = 1; k <= n; k++) {
        B[0] = 0;
        for (j = 1; j <= k; j++)
            B[j] = B[j - 1] + A[k - j];
        E[k] = B[k];
        t = A; A = B; B = t;
    }

    C[0] = 1;
    for (i = 1; i <= n; i++)
        for (j = i; j >= 1; j--)
            C[j] += C[j - 1];

    for (i = 0; i <= n; i++) {
        res += C[i] * p * E[i];
        p += p;
    }

    release(E, n + 1);
    release(A, n + 1);
    release(B, n + 1);
    release(C, n + 1);
    return res;
}
