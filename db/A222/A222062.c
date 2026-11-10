/*
 * A222062  a(n) = n-th second-order hypergeometric-harmonic number.
 * https://oeis.org/A222062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} S2(n,k)*k!*H2(k), where H2(k) = (k+1)*H_k -
 *            k, so k!*H2(k) = (k+1)*F(k) - k*k! with F(k) = k!*H_k, F(k) =
 *            k*F(k-1) + (k-1)!, F(0) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    seq_t *T = mem_new(seq_t, m + 1);
    seq_t *S = mem_new(seq_t, m + 2);
    seq_t fact = 1;   /* k! */
    seq_t F = 0;      /* k! * H_k */
    T[0] = 0;
    for (size_t k = 1; k <= m; k++) {
        seq_t prevfact = fact;          /* (k-1)! */
        F = (seq_t)k * F + prevfact;
        fact = prevfact * (seq_t)k;
        T[k] = ((seq_t)k + 1) * F - (seq_t)k * fact;
    }
    S[0] = 1;
    for (size_t i = 1; i <= m; i++) {
        for (size_t j = i; j >= 1; j--) {
            S[j] = (seq_t)j * S[j] + S[j - 1];
        }
        S[0] = 0;
    }
    seq_t res = 0;
    for (size_t k = 0; k <= m; k++)
        res += S[k] * T[k];
    mem_free(T);
    mem_free(S);
    return res;
}
