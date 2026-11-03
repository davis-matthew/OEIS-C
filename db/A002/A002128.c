/*
 * A002128  MacMahon's generalized sum of divisors function.
 * https://oeis.org/A002128
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(x) = N(x)/(-7*D(x)), where D(x) = Sum_{k>=0}
 *            (-1)^k*(2k+1)*x^(k(k+1)/2) and N(x) = Sum_{k>=0}
 *            (-1)^k*(2k+1)*binomial(k+3,6)*x^(k(k+1)/2). Comparing
 *            coefficients of A(x)*D(x) = -N(x)/7 gives a(m) = [m = k(k+1)/2]
 *            (-1)^(k+1)*(2k+1)*binomial(k+3,6)/7 + Sum_{k>=1}
 *            (-1)^(k+1)*(2k+1)*a(m - k(k+1...
 * Range:     u8 n <= 13, u16 n <= 35, u32 n <= 307, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9127 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

/* (2k+1)*binomial(k+3,6)/7, always an integer (7 is prime and 7 does not divide 720) */
static unsigned long long a002128_e(unsigned long long k) {
    unsigned long long c = 1;
    unsigned long long i;
    if (k < 3) return 0;
    for (i = 1; i <= 6; i++)
        c = c * (k - 3 + i) / i;
    return (2 * k + 1) * c / 7;
}

seq_t a(idx_t n) {
    seq_t *A;
    seq_t result;
    idx_t m, k, T;
    if (n < 6) return 0;
    A = alloc_zero(n + 1);
    for (m = 6; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1; (T = k * (k + 1) / 2) <= m; k++) {
            int plus = (k & 1) ? 1 : 0;
            if (T == m) {
                seq_t e = (seq_t)a002128_e((unsigned long long)k);
                if (plus) pos += e; else neg += e;
            } else {
                seq_t t = (seq_t)(2 * k + 1) * A[m - T];
                if (plus) pos += t; else neg += t;
            }
        }
        A[m] = pos - neg;
    }
    result = A[n];
    release(A, n + 1);
    return result;
}
