/*
 * A000060  Number of signed trees with n nodes.
 * https://oeis.org/A000060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A000151(n) + A000151(n/2) - Sum_{i=1..n-1}
 *            A000151(i)*A000151(n-i), where A000151(n/2) = 0 for odd n (g.f.
 *            S(x) + S(x^2) - S(x)^2), and S(x) = Sum A000151(n) x^n = x *
 *            Prod_{k>=1} (1 - x^k)^(-2*A000151(k)), A000151(1) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_series, g_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_series.h"
#include "../utils/g_gcd.h"

static seq_t *S_ = 0;
static idx_t cap_ = 0;

/* fills S[1..N] with A000151 via the Euler product, division-free except exact binomials */
static void build(idx_t N)
{
    seq_t *F = alloc_series((size_t)N + 1);
    seq_t *B = alloc_series((size_t)N + 1);
    seq_t *S = alloc_series((size_t)N + 1);
    idx_t i, K, s, j, q;
    for (i = 0; i <= N; i++) {
        F[i] = 0;
        B[i] = 0;
        S[i] = 0;
    }
    F[0] = 1;
    S[1] = 1;
    for (K = 1; K < N; K++) {
        seq_t c = S[K] * 2;
        idx_t jm = (N - 1) / K;
        seq_t r = 1;
        /* B[j] = C(c + j - 1, j) */
        for (j = 1; j <= jm; j++) {
            unsigned long long g = g_gcd((unsigned long long)(r % j), (unsigned long long)j);
            r = (r / g) * ((c + (j - 1)) / (seq_t)(j / g));
            B[j] = r;
        }
        for (s = N - 1; s >= K; s--) {
            seq_t t = 0;
            q = s - K;
            j = 1;
            for (;;) {
                t += B[j] * F[q];
                if (q < K) break;
                q -= K;
                j++;
            }
            F[s] += t;
        }
        S[K + 1] = F[K];
    }
    S_ = S;
    cap_ = N;
}

seq_t a(idx_t n)
{
    idx_t i;
    seq_t t;
    if (n > cap_) {
        idx_t N = cap_ * 2;
        if (N < n) N = n;
        if (N < 32) N = 32;
        build(N);
    }
    t = S_[n];
    if (n % 2 == 0) t += S_[n / 2];
    for (i = 1; i < n; i++) t -= S_[i] * S_[n - i];
    return t;
}
