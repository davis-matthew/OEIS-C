/*
 * A002290  Absolute value of Glaisher's alpha(n).
 * https://oeis.org/A002290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = |A225543(n)| = |[q^n] eta(q)^4 eta(q^2)^12 / eta(q^4)^4|
 *            = |[q^n] phi(-q)^2 * phi(-q^2)^4 * f(-q^2)^6|, computed as
 *            phi(-q)^2 * phi(-q^2)^4 * E(q^2)^2, where phi(-x) = Sum_{m in Z}
 *            (-1)^m x^(m^2) and E(x) = f(-x)^3 = Sum_{m>=0} (-1)^m (2m+1)
 *            x^(m(m+1)/2) (Jacobi)
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 2706, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002290_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a002290_mul.h"

static __int128 *a002290_F = 0;
static long long a002290_N = -1;

static void a002290_build(long long N)
{
    __int128 *f = (__int128 *)malloc((size_t)(N + 1) * sizeof(__int128));
    __int128 *t = (__int128 *)malloc((size_t)(N + 1) * sizeof(__int128));
    long long i;
    int k;
    for (i = 0; i <= N; i++) f[i] = 0;
    f[0] = 1;
    for (k = 0; k < 2; k++) a002290_mul(f, t, N, 1, 0);
    for (k = 0; k < 4; k++) a002290_mul(f, t, N, 2, 0);
    for (k = 0; k < 2; k++) a002290_mul(f, t, N, 2, 1);
    free(t);
    if (a002290_F) free(a002290_F);
    a002290_F = f;
    a002290_N = N;
}

seq_t a(idx_t n)
{
    long long nn = (long long)n;
    __int128 v;
    unsigned __int128 u;
    uint64_t lo, hi;
    if (nn > a002290_N) {
        long long N = nn;
        if (a002290_N >= 0 && N < 2 * a002290_N) N = 2 * a002290_N;
        if (N < 64) N = 64;
        a002290_build(N);
    }
    v = a002290_F[nn];
    u = (v < 0) ? (unsigned __int128)(-v) : (unsigned __int128)v;
    lo = (uint64_t)u;
    hi = (uint64_t)(u >> 64);
    if (hi == 0) return (seq_t)lo;
    return (((seq_t)hi << 32) << 32) + (seq_t)lo;
}
