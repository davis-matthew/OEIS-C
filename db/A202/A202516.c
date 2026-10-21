/*
 * A202516  G.f.: exp( Sum_{n>=1} (2^n + 3^n)^n * x^n/n ).
 * https://oeis.org/A202516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (1/n) * Σ_{k=1..n} (2^k + 3^k)^k * a(n-k) (n ≥
 *            1) This is the exponential generating function A(x) = exp(
 *            Σ_{k≥1} (2^k+3^k)^k * x^k / k )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"   /* prototype: seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* static tables with values already computed */
    static seq_t *A   = NULL;   /* A[0..A_size-1] */
    static idx_t   A_sz = 0;
    static seq_t *pow2 = NULL;  /* 2^i   */
    static seq_t *pow3 = NULL;  /* 3^i   */
    static seq_t *S    = NULL;  /* S[i] = (2^i+3^i)^i  */
    static idx_t   S_sz = 0;

    /* grow value table as needed */
    if (A_sz <= n) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t*)realloc(A, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        A = tmp;
        /* initialise new entries (except A[0] which will be set later) */
        for (idx_t i = A_sz; i < new_sz; ++i) A[i] = (seq_t)0;
        A_sz = new_sz;
    }

    /* grow helper tables as needed */
    if (S_sz <= n) {
        idx_t new_sz = n + 1;
        seq_t *tmp2, *tmp3, *tmpS;

        tmp2 = (seq_t*)realloc(pow2, new_sz * sizeof(seq_t));
        tmp3 = (seq_t*)realloc(pow3, new_sz * sizeof(seq_t));
        tmpS = (seq_t*)realloc(S,    new_sz * sizeof(seq_t));
        if (!tmp2 || !tmp3 || !tmpS) abort();

        pow2 = tmp2; pow3 = tmp3; S = tmpS;

        if (S_sz == 0) {          /* first time allocation */
            pow2[0] = (seq_t)1;
            pow3[0] = (seq_t)1;
            S[0]    = (seq_t)0;   /* unused */
            S_sz    = 1;
        }

        for (idx_t i = S_sz; i <= n; ++i) {
            pow2[i] = pow2[i-1] * (seq_t)2;
            pow3[i] = pow3[i-1] * (seq_t)3;
            seq_t base = pow2[i] + pow3[i];
            S[i] = big_pow(base, (seq_t)i);  /* (2^i+3^i)^i */
        }
        S_sz = n + 1;
    }

    /* initialise a(0) once */
    if (A_sz && A[0] == (seq_t)0) A[0] = (seq_t)1;

    /* compute a(k) up to n if not already done */
    for (idx_t k = 1; k <= n; ++k) {
        if (A[k] != (seq_t)0) continue;    /* already computed */

        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= k; ++j) {
            /* term: S[j] * a(k-j) ; S[j] already (2^j+3^j)^j */
            sum += S[j] * A[k-j];
        }
        A[k] = sum / (seq_t)k;
    }

    return A[n];
}
