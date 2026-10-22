/*
 * A002988  Number of trimmed trees with n nodes.
 * https://oeis.org/A002988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Q(x) = x*E(Q)(x) - x^2, where E is the Euler transform:
 *            [x^m]E(Q) = e(m), e(0) = 1, m*e(m) = Sum_{k=1..m} c(k)*e(m-k),
 *            c(k) = Sum_{d|k} d*Q(d). Q counts planted trimmed trees; P(m) =
 *            Q(m) - [m=1]. For n >= 1, a(n) = e(n-1) - P(n-2) - [n=3] -
 *            Sum_{i<j, i+j=n} Q(i)*Q(j) - [n even]*binomial(Q(n/2),2...
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 33, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   A_grow, a2988_fixed, a2988_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/a2988_fixed.h"
#include "../utils/a2988_pw.h"

/* ---------- arbitrary precision path ---------- */
static seq_t *a2988_Q = 0, *a2988_E = 0, *a2988_C = 0;
static idx_t a2988_cap = 0, a2988_have = 0;

static void a2988_extend(idx_t N)
{
    idx_t m;
    if (N + 2 > a2988_cap) {
        idx_t nc = N + 2;
        if (nc < 2 * a2988_cap) nc = 2 * a2988_cap;
        a2988_Q = A_grow(a2988_Q, a2988_cap, nc);
        a2988_E = A_grow(a2988_E, a2988_cap, nc);
        a2988_C = A_grow(a2988_C, a2988_cap, nc);
        a2988_cap = nc;
    }
    if (a2988_have == 0) {
        a2988_E[0] = 1; a2988_Q[0] = 0; a2988_Q[1] = 1; a2988_C[0] = 0;
        a2988_have = 1;
    }
    for (m = a2988_have; m <= N; m++) {
        seq_t c = 0, s = 0;
        idx_t d, k;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t o = m / d;
                c += a2988_Q[d] * (seq_t)d;
                if (o != d) c += a2988_Q[o] * (seq_t)o;
            }
        }
        a2988_C[m] = c;
        for (k = 1; k <= m; k++) s += a2988_C[k] * a2988_E[m - k];
        a2988_E[m] = s / (seq_t)m;
        a2988_Q[m + 1] = a2988_E[m];
        if (m == 1) a2988_Q[m + 1] -= 1;
    }
    if (N + 1 > a2988_have) a2988_have = N + 1;
}

static seq_t a2988_big(idx_t n)
{
    seq_t pos, neg = 0;
    idx_t i;
    if (n == 0) return 1;
    a2988_extend(n - 1);
    pos = a2988_E[n - 1];
    if (n >= 2 && n != 3) neg += a2988_Q[n - 2];
    if (n == 3) neg += 1;
    for (i = 1; 2 * i < n; i++) neg += a2988_Q[i] * a2988_Q[n - i];
    if (n % 2 == 0) {
        seq_t q = a2988_Q[n / 2];
        if (q != 0) neg += q * (q - 1) / 2;
    }
    return pos - neg;
}

/* ---------- fixed-width path: multi-modular + Garner ---------- */
#define A2988_FN 1001
static const uint64_t a2988_P[6] = {
    1000000007ULL, 1000000009ULL, 998244353ULL,
    754974721ULL, 167772161ULL, 469762049ULL
};
static uint64_t a2988_R[6][A2988_FN + 1];
static int a2988_ready = 0;

static void a2988_modrun(int j)
{
    static uint64_t Q[A2988_FN + 2], E[A2988_FN + 2], C[A2988_FN + 2], IV[A2988_FN + 2];
    uint64_t p = a2988_P[j];
    int m, k, d, n, i;
    IV[1] = 1;
    for (i = 2; i <= A2988_FN; i++)
        IV[i] = (p - (p / (uint64_t)i) * IV[p % (uint64_t)i] % p) % p;
    Q[0] = 0; Q[1] = 1; E[0] = 1; C[0] = 0;
    for (m = 1; m < A2988_FN; m++) {
        uint64_t c = 0, s = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                int o = m / d;
                c = (c + Q[d] * (uint64_t)d) % p;
                if (o != d) c = (c + Q[o] * (uint64_t)o) % p;
            }
        }
        C[m] = c;
        for (k = 1; k <= m; k++) s = (s + C[k] * E[m - k]) % p;
        E[m] = s * IV[m] % p;
        Q[m + 1] = (E[m] + p - (m == 1 ? 1 : 0)) % p;
    }
    a2988_R[j][0] = 1;
    for (n = 1; n <= A2988_FN; n++) {
        uint64_t pos = E[n - 1], neg = 0;
        if (n >= 2 && n != 3) neg = Q[n - 2];
        if (n == 3) neg = (neg + 1) % p;
        for (i = 1; 2 * i < n; i++) neg = (neg + Q[i] * Q[n - i]) % p;
        if (n % 2 == 0) {
            uint64_t q = Q[n / 2];
            neg = (neg + q * ((q + p - 1) % p) % p * IV[2]) % p;
        }
        a2988_R[j][n] = (pos + p - neg) % p;
    }
}

static seq_t a2988_small(idx_t n)
{
    uint64_t t[6];
    unsigned __int128 x;
    uint64_t lo, hi;
    seq_t r = 0;
    int i, j;
    if (n > A2988_FN) return 0; /* far beyond any fixed width */
    if (!a2988_ready) {
        for (j = 0; j < 6; j++) a2988_modrun(j);
        a2988_ready = 1;
    }
    for (j = 0; j < 6; j++) {
        uint64_t pj = a2988_P[j];
        uint64_t v = a2988_R[j][n];
        for (i = 0; i < j; i++) {
            uint64_t inv = a2988_pw(a2988_P[i] % pj, pj - 2, pj);
            v = (v + pj - t[i] % pj) % pj * inv % pj;
        }
        t[j] = v;
    }
    x = t[5];
    for (j = 4; j >= 0; j--) x = x * a2988_P[j] + t[j];
    lo = (uint64_t)x;
    hi = (uint64_t)(x >> 64);
    if (sizeof(seq_t) > 8) {
        r = (seq_t)hi;
        r <<= 32;
        r <<= 32;
    }
    r += (seq_t)lo;
    return r;
}

seq_t a(idx_t n)
{
    if (a2988_fixed()) return a2988_small(n);
    return a2988_big(n);
}
