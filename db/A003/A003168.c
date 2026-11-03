/*
 * A003168  Number of blobs with 2n+1 edges.
 * https://oeis.org/A003168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} binomial(n, k)*binomial(2*n+k, k-1)/n, a(0)
 *            = 1 (big integers). Fixed-width builds use the equivalent
 *            division-free g.f. relation P = 1 + z*(2*P^3 - P^2), P(z) = Sum
 *            a(n) z^n, i.e. a(n) = [z^(n-1)](2*P^3 - P^2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 999 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t a003168_big(idx_t n) {
    /* term_k = binomial(n,k)*binomial(2n+k,k-1); term_1 = n
       term_{k+1} = term_k * (n-k)*(2n+k+1) / (k*(k+1)) exactly */
    seq_t term = (seq_t)n;
    seq_t sum = 0;
    idx_t k;
    for (k = 1; k <= n; k++) {
        sum += term;
        if (k == n) break;
        term *= (seq_t)(n - k);
        term *= (seq_t)(2 * n + k + 1);
        term /= (seq_t)k;
        term /= (seq_t)(k + 1);
    }
    return sum / (seq_t)n;
}

static seq_t a003168_fixed(idx_t n) {
    seq_t *A = mem_new(seq_t, (size_t)(n + 1));
    seq_t *Q = mem_new(seq_t, (size_t)(n + 1));
    seq_t *R = mem_new(seq_t, (size_t)(n + 1));
    idx_t m, i;
    seq_t res;
    A[0] = 1; Q[0] = 1; R[0] = 1;
    for (m = 1; m <= n; m++) {
        A[m] = R[m - 1] * 2 - Q[m - 1];
        if (m == n) break;
        {
            seq_t q = 0;
            for (i = 0; i <= m; i++) q += A[i] * A[m - i];
            Q[m] = q;
        }
        {
            seq_t r = 0;
            for (i = 0; i <= m; i++) r += Q[i] * A[m - i];
            R[m] = r;
        }
    }
    res = A[n];
    mem_free(A); mem_free(Q); mem_free(R);
    return res;
}

static bool a003168_is_fixed(void) {
    seq_t m1 = 0;
    m1 -= 1;
    return m1 > 0;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (a003168_is_fixed()) return a003168_fixed(n);
    return a003168_big(n);
}
