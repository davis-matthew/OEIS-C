/*
 * A007240  McKay-Thompson series of class 1A for the Monster group with a(0)
 *          = 24.
 * https://oeis.org/A007240
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [q^n] j(q) - 720*[n=0], with j(q) = E4(q)^3 / Delta(q),
 *            computed as a(n) = Sum_{k=0..n+1} e(k)*g(n+1-k) - 720*[n=0],
 *            where g = coefficients of Product_{k>=1} (1-q^k)^(-24), obtained
 *            from the sparse recurrence m*g(m) = -Sum_{k>=1}
 *            b_k*(m+7*t_k)*g(m-t_k) with eta(q)^3 = Sum_k b_k q^(t_k), b_...
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 13, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_zero, mulpow, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/mulpow.h"

/* ---------- arbitrary-precision path ---------- */
static seq_t *G = NULL, *E = NULL;
static idx_t glen = 0;

static void big_build(idx_t need) {
    idx_t N, m, k, d, t, j;
    __int128 *D;
    seq_t *S;
    if (need <= glen) return;
    N = need;
    if (N < 2 * glen) N = 2 * glen;
    if (N < 64) N = 64;
    if (glen > 0) {
        release(G, glen);
        release(E, glen);
    }
    G = alloc_zero(N);
    E = alloc_zero(N);
    D = (__int128 *)calloc((size_t)N, sizeof(__int128));
    G[0] = 1;
    D[0] = 1;
    for (m = 1; m < N; m++) {
        seq_t pos = 0, neg = 0;
        __int128 ds = 0;
        for (k = 1; (t = k * (k + 1) / 2) <= m; k++) {
            idx_t bk = (k & 1) ? -(2 * k + 1) : (2 * k + 1);
            idx_t c = bk * (m + 7 * t);
            if (c < 0) pos += G[m - t] * (-c);
            else neg += G[m - t] * c;
            ds += (__int128)bk * (__int128)(9 * t - m) * D[m - t];
        }
        G[m] = (pos - neg) / m;
        D[m] = ds / m;
    }
    S = alloc_zero(N);
    for (d = 1; d < N; d++) {
        seq_t p = 1;
        for (j = 0; j < 11; j++) p *= d;
        for (t = d; t < N; t += d) S[t] += p;
    }
    E[0] = 1;
    for (k = 1; k < N; k++) {
        __int128 tau = D[k - 1];
        seq_t x = S[k] * 65520;
        if (tau >= 0) x += u128_to_seq((unsigned __int128)tau) * 432000;
        else x -= u128_to_seq((unsigned __int128)(-tau)) * 432000;
        E[k] = x / 691;
    }
    release(S, N);
    free(D);
    glen = N;
}

/* ---------- fixed-width path ---------- */
static seq_t *Pm = NULL, *P24 = NULL, *P48 = NULL;
static idx_t plen = 0;

static void fixed_extend(idx_t L) {
    idx_t newlen, k;
    if (L <= plen) return;
    newlen = L;
    if (newlen < 2 * plen) newlen = 2 * plen;
    if (newlen < 64) newlen = 64;
    if (plen > 0) {
        release(Pm, plen);
        release(P24, plen);
        release(P48, plen);
    }
    Pm = alloc_zero(newlen);
    P24 = alloc_zero(newlen);
    P48 = alloc_zero(newlen);
    Pm[0] = 1; P24[0] = 1; P48[0] = 1;
    for (k = 1; k < newlen; k++) {
        mulpow(P24, newlen, k, 24, 0);
        mulpow(P48, newlen, k, 48, 0);
        if (k & 1) mulpow(Pm, newlen, k, 24, 1);
    }
    plen = newlen;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t k;
    if (n < -1) return 0;
    if (seq_is_big()) {
        big_build(n + 2);
        for (k = 0; k <= n + 1; k++) r += E[k] * G[n + 1 - k];
        if (n == 0) r -= 720;
        return r;
    }
    fixed_extend(n + 2);
    r = Pm[n + 1];
    if (n == 0) r += 48;
    if (n >= 1) r += P24[n - 1] * 196608;
    if (n >= 2) r += P48[n - 2] * 16777216;
    return r;
}
