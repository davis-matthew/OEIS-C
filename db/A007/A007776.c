/*
 * A007776  Number of connected posets with n elements of height 1.
 * https://oeis.org/A007776
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse Euler transform of A049312 (bicolored graphs, two
 *            distinguished colour classes), with Euler transform c(1)=2,
 *            c(n)=a(n) for n>=2. A049312(n) is computed by Burnside:
 *            A049312(n) = Sum_{p+q=n} Sum_{lambda |- p} (1/z_lambda) [t^q]
 *            exp( Sum_{m>=1} 2^{Sum_i gcd(lambda_i,m)} t^m/m ), i.e...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 20, u128 n <= 28,
 *            big any n
 * Helpers:   A7776_gcd, alloc_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/A7776_gcd.h"

typedef unsigned long long A7776_u64;

static seq_t *A7776_cache = NULL;
static idx_t A7776_cache_n = 0;

static idx_t W_N, W_K, W_p, W_len, W_R;
static A7776_u64 *W_pr, *W_invi, *W_pw, *W_cnt, *W_x, *W_P;
static idx_t *W_cyc, *W_parts;

static A7776_u64 A7776_powmod(A7776_u64 b, A7776_u64 e, A7776_u64 m)
{
    A7776_u64 r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

static bool A7776_isprime(A7776_u64 x)
{
    A7776_u64 d;
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static void A7776_leaf(void)
{
    idx_t N = W_N, Q = N - W_p, L = W_len, m, q, i, j, k;
    for (m = 1; m <= Q; m++) {
        idx_t c = 0;
        i = 0;
        while (i < L) {
            idx_t part = W_parts[i];
            j = i;
            while (j < L && W_parts[j] == part) j++;
            c += (j - i) * A7776_gcd(part, m);
            i = j;
        }
        W_cyc[m] = c;
    }
    for (k = 0; k < W_K; k++) {
        A7776_u64 P = W_pr[k], M2 = P * P, zinv = 1;
        A7776_u64 *invi = W_invi + k * (N + 1);
        A7776_u64 *pw = W_pw + k * (N + 1);
        A7776_u64 *cnt = W_cnt + k * W_R * (N + 1) + W_p * (N + 1);
        i = 0;
        while (i < L) {
            idx_t part = W_parts[i], mult = 0;
            j = i;
            while (j < L && W_parts[j] == part) {
                mult++;
                zinv = zinv * invi[part] % P * invi[mult] % P;
                j++;
            }
            i = j;
        }
        for (m = 1; m <= Q; m++) W_x[m] = pw[W_cyc[m]];
        W_P[0] = 1;
        for (q = 1; q <= Q; q++) {
            A7776_u64 acc = 0;
            for (m = 1; m <= q; m++) {
                acc += W_x[m] * W_P[q - m];
                if (acc >= M2) acc -= M2;
            }
            W_P[q] = acc % P * invi[q] % P;
        }
        for (q = W_p; q <= Q; q++)
            cnt[q] = (cnt[q] + zinv * W_P[q]) % P;
    }
}

static void A7776_gen(idx_t rem, idx_t maxp)
{
    idx_t part;
    if (rem == 0) { A7776_leaf(); return; }
    for (part = (rem < maxp ? rem : maxp); part >= 1; part--) {
        W_parts[W_len++] = part;
        A7776_gen(rem - part, part);
        W_len--;
    }
}

static void A7776_compute(idx_t N)
{
    idx_t K, k, i, j, p, q, kk;
    A7776_u64 cand;
    A7776_u64 *res, *b, *s, *invp, *v;
    long long *mu;

    K = (N * N / 4 + 40) / 30 + 1;
    if (seq_is_fixed_width() && K > 5) K = 5;
    W_N = N; W_K = K; W_R = N / 2 + 1;

    W_pr = (A7776_u64 *)malloc(K * sizeof(A7776_u64));
    cand = 2147483647ULL;
    for (k = 0; k < K; k++) {
        while (!A7776_isprime(cand)) cand--;
        W_pr[k] = cand;
        cand--;
    }
    W_invi = (A7776_u64 *)malloc(K * (N + 1) * sizeof(A7776_u64));
    W_pw = (A7776_u64 *)malloc(K * (N + 1) * sizeof(A7776_u64));
    W_cnt = (A7776_u64 *)calloc(K * W_R * (N + 1), sizeof(A7776_u64));
    W_x = (A7776_u64 *)malloc((N + 1) * sizeof(A7776_u64));
    W_P = (A7776_u64 *)malloc((N + 1) * sizeof(A7776_u64));
    W_cyc = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    W_parts = (idx_t *)malloc((N + 1) * sizeof(idx_t));

    for (k = 0; k < K; k++) {
        A7776_u64 P = W_pr[k];
        A7776_u64 *invi = W_invi + k * (N + 1), *pw = W_pw + k * (N + 1);
        invi[0] = 0;
        if (N >= 1) invi[1] = 1;
        for (i = 2; i <= N; i++)
            invi[i] = (P - (P / i) * invi[P % i] % P) % P;
        pw[0] = 1;
        for (i = 1; i <= N; i++) pw[i] = pw[i - 1] * 2 % P;
    }

    for (p = 0; p <= N / 2; p++) {
        W_p = p;
        W_len = 0;
        A7776_gen(p, p);
    }

    mu = (long long *)calloc(N + 1, sizeof(long long));
    mu[1] = 1;
    for (i = 1; i <= N; i++)
        for (j = 2 * i; j <= N; j += i) mu[j] -= mu[i];

    res = (A7776_u64 *)calloc((N + 1) * K, sizeof(A7776_u64));
    b = (A7776_u64 *)malloc((N + 1) * sizeof(A7776_u64));
    s = (A7776_u64 *)malloc((N + 1) * sizeof(A7776_u64));
    for (k = 0; k < K; k++) {
        A7776_u64 P = W_pr[k];
        A7776_u64 *invi = W_invi + k * (N + 1);
        A7776_u64 *cnt = W_cnt + k * W_R * (N + 1);
        for (kk = 0; kk <= N; kk++) {
            A7776_u64 t = 0;
            for (p = 0; p <= kk; p++) {
                idx_t lo = p, hi = kk - p;
                if (lo > hi) { idx_t tt = lo; lo = hi; hi = tt; }
                t = (t + cnt[lo * (N + 1) + hi]) % P;
            }
            b[kk] = t;
        }
        for (kk = 1; kk <= N; kk++) {
            A7776_u64 t = (kk % P) * b[kk] % P;
            for (j = 1; j < kk; j++)
                t = (t + P - s[j] * b[kk - j] % P) % P;
            s[kk] = t;
        }
        for (kk = 1; kk <= N; kk++) {
            A7776_u64 t = 0;
            for (j = 1; j <= kk; j++) {
                if (kk % j) continue;
                if (mu[kk / j] == 1) t = (t + s[j]) % P;
                else if (mu[kk / j] == -1) t = (t + P - s[j]) % P;
            }
            res[kk * K + k] = t * invi[kk] % P;
        }
    }

    invp = (A7776_u64 *)malloc(K * K * sizeof(A7776_u64));
    for (i = 0; i < K; i++)
        for (j = 0; j < i; j++)
            invp[j * K + i] = A7776_powmod(W_pr[j] % W_pr[i], W_pr[i] - 2, W_pr[i]);
    v = (A7776_u64 *)malloc(K * sizeof(A7776_u64));

    A7776_cache = alloc_zero(N + 1);
    for (kk = 1; kk <= N; kk++) {
        seq_t x;
        for (i = 0; i < K; i++) {
            A7776_u64 Pi = W_pr[i], t = res[kk * K + i];
            for (j = 0; j < i; j++)
                t = (t + Pi - v[j] % Pi) % Pi * invp[j * K + i] % Pi;
            v[i] = t;
        }
        x = (seq_t)(idx_t)v[K - 1];
        for (i = K - 1; i > 0; i--)
            x = x * (seq_t)(idx_t)W_pr[i - 1] + (seq_t)(idx_t)v[i - 1];
        A7776_cache[kk] = x;
    }
    A7776_cache_n = N;

    free(v); free(invp); free(res); free(b); free(s); free(mu);
    free(W_pr); free(W_invi); free(W_pw); free(W_cnt);
    free(W_x); free(W_P); free(W_cyc); free(W_parts);
}

seq_t a(idx_t n)
{
    idx_t N = n;
    if (seq_is_fixed_width()) {
        if (n > 40) return (seq_t)0;
        N = 40;
    }
    if (A7776_cache == NULL || n > A7776_cache_n)
        A7776_compute(N);
    return A7776_cache[n];
}
