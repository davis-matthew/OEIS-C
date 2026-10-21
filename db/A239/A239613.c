/*
 * A239613  a(n) = Sum_{0 < x,y,z,t <= n and gcd(x^2 + y^2 + z^2 + t^2, n)=1}
 *          gcd(x^2 + y^2 + z^2 + t^2 - 1, n).
 * https://oeis.org/A239613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{0 < x,y,z,t <= n, gcd(x^2+y^2+z^2+t^2 , n)=1} *
 *            gcd(x^2+y^2+z^2+t^2 - 1 , n)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;

    /* Count residues of squares modulo n */
    uint32_t *cnt = (uint32_t *)malloc(N * sizeof(uint32_t));
    if (!cnt) return 0;
    for (size_t i = 0; i < N; ++i) cnt[i] = 0;
    for (idx_t x = 1; x <= n; ++x) {
        idx_t r = (x * x) % n;
        cnt[(size_t)r] += 1;
    }

    /* First convolution: cnt * cnt -> A  (uint64_t) */
    uint64_t *A = (uint64_t *)calloc(N, sizeof(uint64_t));
    if (!A) { free(cnt); return 0; }
    for (size_t i = 0; i < N; ++i) {
        if (cnt[i] == 0) continue;
        uint32_t ci = cnt[i];
        for (size_t j = 0; j < N; ++j) {
            if (cnt[j] == 0) continue;
            size_t idx = i + j;
            if (idx >= N) idx -= N;               /* modulo n */
            A[idx] += (uint64_t)ci * cnt[j];
        }
    }

    /* Second convolution: A * cnt -> B  (uint64_t) */
    uint64_t *B = (uint64_t *)calloc(N, sizeof(uint64_t));
    if (!B) { free(cnt); free(A); return 0; }
    for (size_t i = 0; i < N; ++i) {
        if (A[i] == 0) continue;
        uint64_t ai = A[i];
        for (size_t j = 0; j < N; ++j) {
            if (cnt[j] == 0) continue;
            size_t idx = i + j;
            if (idx >= N) idx -= N;
            B[idx] += ai * cnt[j];
        }
    }

    /* Third convolution: B * cnt -> C  (uint64_t) */
    uint64_t *C = (uint64_t *)calloc(N, sizeof(uint64_t));
    if (!C) { free(cnt); free(A); free(B); return 0; }
    for (size_t i = 0; i < N; ++i) {
        if (B[i] == 0) continue;
        uint64_t bi = B[i];
        for (size_t j = 0; j < N; ++j) {
            if (cnt[j] == 0) continue;
            size_t idx = i + j;
            if (idx >= N) idx -= N;
            C[idx] += bi * cnt[j];
        }
    }

    /* Accumulate answer */
    seq_t ans = 0;
    for (size_t s = 0; s < N; ++s) {
        if (gcd((seq_t)s, (seq_t)n) == 1) {
            size_t r = s + N - 1;
            if (r >= N) r -= N;                     /* (s-1) mod n */
            seq_t g = gcd((seq_t)r, (seq_t)n);
            ans += (seq_t)C[s] * g;
        }
    }

    free(cnt);
    free(A);
    free(B);
    free(C);
    return ans;
}
