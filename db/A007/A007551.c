/*
 * A007551  Shifts left when Moebius transformation applied twice.
 * https://oeis.org/A007551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, eigen
 * Formula:   a(1) = 1; a(n+1) = Sum_{d|n} mu2(n/d)*a(d), where mu2 = mu*mu is
 *            the Dirichlet square of the Moebius function: multiplicative
 *            with mu2(p) = -2, mu2(p^2) = 1, mu2(p^e) = 0 for e >= 3
 * Range:     i8 n <= 28, i16 n <= 100, i32 n <= 732, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   alloc_zero, mu2, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mu2.h"

static seq_t *A_tab = NULL;
static idx_t A_cap = 0;

static void build(idx_t N)
{
    seq_t *arr = alloc_zero(N + 1);
    seq_t *acc = alloc_zero(N + 1);
    long long *mu = (long long *)malloc((size_t)(N + 1) * sizeof(long long));
    idx_t d, k;
    for (k = 1; k <= N; k++) mu[k] = mu2(k);
    arr[1] = 1;
    for (d = 1; d <= N; d++) {
        if (d > 1) arr[d] = acc[d - 1];
        for (k = 1; d * k <= N - 1; k++) {
            long long m = mu[k];
            if (m != 0) acc[d * k] += arr[d] * (seq_t)m;
        }
    }
    free(mu);
    release(acc, N + 1);
    if (A_tab) release(A_tab, A_cap + 1);
    A_tab = arr;
    A_cap = N;
}

seq_t a(idx_t n)
{
    if (n > A_cap) {
        idx_t N = A_cap * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build(N);
    }
    return A_tab[n];
}
