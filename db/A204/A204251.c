/*
 * A204251  Permanent of the n-th principal submatrix of A204250.
 * https://oeis.org/A204251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-3)^(n-k) * (n-k)! * k! *
 *            e_k(2,3,...,n+1)^2, where e_k is the k-th elementary symmetric
 *            polynomial. This is perm of the n X n matrix ((i+1)(j+1)-3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t *e = mem_new(seq_t, (size_t)n + 1);
    seq_t *fact = mem_new(seq_t, (size_t)n + 1);
    seq_t *p3 = mem_new(seq_t, (size_t)n + 1);
    e[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t x = (seq_t)i + 1;
        for (idx_t k = i; k >= 1; k--) {
            e[k] = e[k] + e[k - 1] * x;
        }
    }
    fact[0] = 1;
    p3[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * (seq_t)i;
        p3[i] = p3[i - 1] * 3;
    }
    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k <= n; k++) {
        idx_t m = n - k;
        seq_t t = p3[m] * fact[m] * fact[k] * e[k] * e[k];
        if (m & 1) neg = neg + t; else pos = pos + t;
    }
    seq_t r = pos - neg;
    mem_free(e);
    mem_free(fact);
    mem_free(p3);
    return r;
}
