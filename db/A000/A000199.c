/*
 * A000199  Coefficient of q^(2n-1) in the series expansion of Ramanujan's
 *          mock theta function f(q).
 * https://oeis.org/A000199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   f(q) = Sum_{j>=0} q^(j^2) / Prod_{k=1..j} (1+q^k)^2, computed in
 *            nested (Horner) form f = 1 + q/(1+q)^2 * (1 + q^3/(1+q^2)^2 * (1
 *            + q^5/(1+q^3)^2 * (...))); a(n) = [q^(2n-1)] f(q). Signed
 *            coefficients are carried as differences P - N of nonnegative
 *            series, so only additions are needed
 * Range:     u8 n <= 20, u16 n <= 61, u32 n <= 204, u64 n <= 721,
 *            u128 n <= 2669, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_series.h"

static seq_t *cache = 0;
static idx_t cacheN = 0;

static void build(idx_t N)
{
    idx_t M = 2 * N - 1;
    seq_t *P = alloc_series((size_t)(M + 1));
    seq_t *Q = alloc_series((size_t)(M + 1));
    idx_t i, j, J, r;
    for (i = 0; i <= M; i++) { P[i] = 0; Q[i] = 0; }
    J = 1;
    while ((J + 1) * (J + 1) <= M) J++;
    P[0] = 1;
    for (j = J; j >= 1; j--) {
        idx_t len = M - j * j + 1;  /* coefficients 0..M-j^2 of the inner series */
        idx_t s = 2 * j - 1;
        /* divide by (1+q^j)^2: b[i] = a[i] - b[i-j], twice */
        for (r = 0; r < 2; r++) {
            for (i = j; i < len; i++) {
                P[i] += Q[i - j];
                Q[i] += P[i - j];
            }
        }
        /* multiply by q^(2j-1) and add 1 */
        for (i = len; i-- > 0;) {
            P[i + s] = P[i];
            Q[i + s] = Q[i];
        }
        for (i = 1; i < s; i++) { P[i] = 0; Q[i] = 0; }
        P[0] = 1;
        Q[0] = 0;
    }
    cache = alloc_series((size_t)(N + 1));
    cache[0] = 0;
    for (i = 1; i <= N; i++) {
        seq_t v = P[2 * i - 1];
        v -= Q[2 * i - 1];
        cache[i] = v;
    }
    cacheN = N;
}

seq_t a(idx_t n)
{
    if (n > cacheN) {
        idx_t N = cacheN + cacheN / 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build(N);
    }
    return cache[n];
}
