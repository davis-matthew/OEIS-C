/*
 * A001461  Partial sums of A006206.
 * https://oeis.org/A001461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=1..n} A006206(k), A006206(k) = (1/k)*Sum_{d|k}
 *            mu(k/d)*L(d), L = Lucas numbers, L(1) = 1, L(2) = 3, L(m) =
 *            L(m-1) + L(m-2). For fixed-width seq_t, k*A006206(k) is rebuilt
 *            from its value mod 2^W and mod p = 2^61-1 (CRT) before the exact
 *            division by k
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 52, u64 n <= 99, u128 n <= 193,
 *            big any n
 * Helpers:   seq_w, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/zalloc_arr.h"
#include "../utils/seq_w.h"

#define P61 2305843009213693951ULL

static uint64_t mulp(uint64_t x, uint64_t y) {
    unsigned __int128 r = (unsigned __int128)x * y;
    uint64_t lo = (uint64_t)(r & P61), hi = (uint64_t)(r >> 61);
    uint64_t s = lo + hi;
    if (s >= P61) s -= P61;
    return s;
}

static uint64_t powp(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    b %= P61;
    while (e) {
        if (e & 1) r = mulp(r, b);
        b = mulp(b, b);
        e >>= 1;
    }
    return r;
}

static seq_t *A_cache = 0;
static idx_t A_cap = 0;

static void build(idx_t N) {
    int W = seq_w();
    int *mu = (int *)calloc((size_t)N + 1, sizeof(int));
    uint64_t *Lp = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    uint64_t *Xp = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    seq_t *Lw = zalloc_arr(N + 1);
    seq_t *pos = zalloc_arr(N + 1);
    seq_t *neg = zalloc_arr(N + 1);
    seq_t *A = zalloc_arr(N + 1);
    idx_t i, d, k;

    mu[1] = 1;
    for (i = 1; i <= N; i++)
        for (k = 2 * i; k <= N; k += i) mu[k] -= mu[i];

    Lw[1] = 1; Lp[1] = 1;
    if (N >= 2) { Lw[2] = 3; Lp[2] = 3; }
    for (i = 3; i <= N; i++) {
        Lw[i] = Lw[i - 1] + Lw[i - 2];
        Lp[i] = (Lp[i - 1] + Lp[i - 2]) % P61;
    }

    for (d = 1; d <= N; d++) {
        for (k = d; k <= N; k += d) {
            int m = mu[k / d];
            if (m == 1) {
                pos[k] += Lw[d];
                Xp[k] = (Xp[k] + Lp[d]) % P61;
            } else if (m == -1) {
                neg[k] += Lw[d];
                Xp[k] = (Xp[k] + P61 - Lp[d]) % P61;
            }
        }
    }

    uint64_t inv2W = 1;
    if (W) inv2W = powp(powp(2, (uint64_t)W), P61 - 2);

    A[0] = 0;
    for (k = 1; k <= N; k++) {
        seq_t X0 = pos[k] - neg[k];
        seq_t b;
        if (W == 0) {
            b = X0 / k;
        } else {
            uint64_t X0p;
            if (W <= 64) X0p = ((uint64_t)X0) % P61;
            else X0p = (uint64_t)(X0 % (seq_t)P61);
            uint64_t t = mulp((Xp[k] + P61 - X0p) % P61, inv2W);
            seq_t M = 0;
            M--;
            seq_t Q = M / k;
            uint64_t R = (uint64_t)(M % k) + 1;
            if (R == k) { Q++; R = 0; }
            seq_t q0 = X0 / k;
            uint64_t r0 = (uint64_t)(X0 % k);
            b = Q * t + q0;
            b += (t * R + r0) / k;
        }
        A[k] = A[k - 1] + b;
    }

    free(mu); free(Lp); free(Xp);
    free(Lw); free(pos); free(neg);
    A_cache = A;
    A_cap = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > A_cap) {
        idx_t nc = A_cap * 2;
        if (nc < n) nc = n;
        if (nc < 64) nc = 64;
        build(nc);
    }
    return A_cache[n];
}
