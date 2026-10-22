/*
 * A000106  2nd power of rooted tree enumerator; number of linear forests of 2
 *          rooted trees.
 * https://oeis.org/A000106
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=1..n-1} r(k)*r(n-k), where r = A000081 (rooted
 *            trees), r(n+1) = [x^n] Prod_{k>=1} (1-x^k)^(-r(k)), r(1) = 1.
 *            The Euler product is expanded factor by factor without division,
 *            using (1-x^k)^(-c) = Sum_{j>=0} binomial(c+j-1, j) x^(k*j), so
 *            every intermediate value stays no larger than...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   a106_gcd, alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"
#include "../utils/a106_gcd.h"

static seq_t *A106_R = 0;   /* A106_R[1..A106_have] = A000081 */
static idx_t A106_have = 0;

static void a106_build(idx_t N)
{
    seq_t *F = alloc_series((size_t)N + 1);
    seq_t *Rn = alloc_series((size_t)N + 1);
    seq_t *C = alloc_series((size_t)N + 1);
    idx_t i, j, k;
    for (i = 0; i <= N; i++) { F[i] = 0; Rn[i] = 0; C[i] = 0; }
    F[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t c;
        idx_t jmax;
        Rn[k] = F[k - 1];
        if (k == N) break;
        c = Rn[k];
        jmax = (N - 1) / k;
        C[0] = 1;
        for (j = 1; j <= jmax; j++) {
            /* C[j] = C[j-1] * (c + j - 1) / j, exactly, without overflow */
            seq_t prev = C[j - 1];
            unsigned long long pm = (unsigned long long)(prev % (seq_t)j);
            unsigned long long g = a106_gcd(pm, (unsigned long long)j);
            unsigned long long q = (unsigned long long)j / g;
            seq_t top = c + (seq_t)(j - 1);
            C[j] = (prev / (seq_t)g) * (top / (seq_t)q);
        }
        /* F *= (1 - x^k)^(-c), truncated to indices 0..N-1, in place */
        for (i = N - 1; i >= k; i--) {
            seq_t s = F[i];
            for (j = 1; j * k <= i; j++)
                s += C[j] * F[i - j * k];
            F[i] = s;
        }
    }
    A106_R = Rn;
    A106_have = N;
}

seq_t a(idx_t n)
{
    idx_t need, k;
    seq_t s = 0;
    if (n < 2) return 0;
    need = n - 1;
    if (A106_have < need) {
        idx_t N = need;
        if (N < 2 * A106_have) N = 2 * A106_have;
        a106_build(N);
    }
    for (k = 1; k <= n - 1; k++)
        s += A106_R[k] * A106_R[n - k];
    return s;
}
