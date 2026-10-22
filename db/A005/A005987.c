/*
 * A005987  Number of symmetric plane partitions of n.
 * https://oeis.org/A005987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f. Product_{i>=1} 1/(1-x^(2i-1)) * Product_{i>=1}
 *            1/(1-x^(2i))^floor(i/2). a(n) = Sum_{k=0..floor(n/2)}
 *            q(n-2k)*e(k), where q = partitions into odd parts (computed by
 *            the pentagonal-number recurrence from q(x)*Prod(1-x^i) =
 *            Prod(1-x^(2i))) and e(k) = [y^k] Prod_{i>=1}
 *            (1-y^i)^(-floor(i/2)) (com...
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 109, u64 n <= 291,
 *            u128 n <= 802, big any n
 * Helpers:   alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t *Qa = 0;
static seq_t *Ea = 0;
static idx_t qcap = 0, ecap = 0;
static bool have = false;

static void build(idx_t n) {
    idx_t N = n / 2;
    seq_t *Q = alloc_zero(n + 1);
    seq_t *E = alloc_zero(N + 1);
    Q[0] = 1;
    for (idx_t j = 1; j <= n; j++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t p1 = k * (3 * k - 1) / 2;
            if (p1 > j) break;
            idx_t p2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += Q[j - p1];
                if (p2 <= j) pos += Q[j - p2];
            } else {
                neg += Q[j - p1];
                if (p2 <= j) neg += Q[j - p2];
            }
            if (k * (3 * k - 1) == j || k * (3 * k + 1) == j) {
                if (k & 1) neg += 1; else pos += 1;
            }
        }
        Q[j] = pos - neg;
    }

    E[0] = 1;
    if (seq_is_big()) {
        uint64_t *b = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
        for (idx_t d = 1; d <= N; d++) {
            uint64_t w = (uint64_t)d * (uint64_t)(d / 2);
            if (w == 0) continue;
            for (idx_t j = d; j <= N; j += d) b[j] += w;
        }
        for (idx_t k = 1; k <= N; k++) {
            seq_t s = 0;
            for (idx_t j = 1; j <= k; j++) {
                if (b[j]) s += E[k - j] * (seq_t)b[j];
            }
            E[k] = s / (seq_t)k;
        }
        free(b);
    } else if (N >= 2) {
        idx_t T0 = N / 2;
        seq_t *B = alloc_zero(T0 + 1);
        for (idx_t t = 0; t <= T0; t++) B[t] = 1;
        for (idx_t i = 2; i <= N; i++) {
            idx_t T = N / i;
            if ((i & 1) == 0 && i >= 4) {
                /* m -> m+1: binom(m+t,t) = Sum_{s<=t} binom(m-1+s,s) */
                for (idx_t t = 1; t <= T; t++) B[t] += B[t - 1];
            }
            for (idx_t k = N; k >= i; k--) {
                seq_t s = 0;
                idx_t tmax = k / i;
                for (idx_t t = 1; t <= tmax; t++) s += B[t] * E[k - t * i];
                E[k] += s;
            }
        }
        release(B, T0 + 1);
    }

    if (have) {
        release(Qa, qcap + 1);
        release(Ea, ecap + 1);
    }
    Qa = Q; Ea = E; qcap = n; ecap = N; have = true;
}

seq_t a(idx_t n) {
    if (!have || n > qcap) build(n < 16 ? 16 : n);
    seq_t r = 0;
    for (idx_t k = 0; 2 * k <= n; k++) r += Qa[n - 2 * k] * Ea[k];
    return r;
}
