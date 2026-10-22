/*
 * A007267  Expansion of 16 * (1 + k^2)^4 /(k * k'^2)^2 in powers of q where k
 *          is the Jacobian elliptic modulus, k' the complementary modulus and
 *          q is the nome.
 * https://oeis.org/A007267
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (-1)^(n+1)*D(n+1) + 4096*E(n-1) + 128*[n=0], where Sum
 *            D(m) x^m = Product_{k>=1} (1+x^(2k-1))^24 = theta_3(x)^8 /
 *            psi(-x)^8, Sum E(m) x^m = Product_{k>=1} (1-x^(2k-1))^(-24) =
 *            psi(x)^8 / theta_4(x)^8. theta_3(x)^8 = Sum r8(m) x^m, r8(m) =
 *            16*Sum_{d|m} (-1)^(m+d) d^3; psi(x)^8 = Sum t8(m) x...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 28, u128 n <= 108,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

static idx_t cap = 0, done = 0;
static long long *R8 = 0, *P8 = 0;
static seq_t *V = 0, *W = 0;                 /* 1/theta_4^8, 1/psi(-x)^8 */
static unsigned __int128 *V2 = 0, *W2 = 0;   /* same, fixed-width builds */

static void tables(idx_t L)
{
    idx_t d, j, e;
    free(R8); free(P8);
    R8 = (long long *)calloc((size_t)L, sizeof(long long));
    P8 = (long long *)calloc((size_t)L, sizeof(long long));
    for (d = 1; d < L; d++) {
        long long d3 = (long long)d * d * d;
        for (j = d; j < L; j += d) {
            if (((j + d) & 1) == 0) R8[j] += d3;
            else R8[j] -= d3;
        }
    }
    for (j = 1; j < L; j++) R8[j] *= 16;
    R8[0] = 1;
    for (d = 1; d <= L; d++) {
        long long d3 = (long long)d * d * d;
        for (e = 1; d * e <= L; e += 2) P8[d * e - 1] += d3;
    }
}

static void ensure(idx_t need)
{
    idx_t i, j, s;
    bool big = seq_is_big() != 0;
    if (need > cap) {
        idx_t nc = need;
        if (nc < 2 * cap) nc = 2 * cap;
        if (nc < 64) nc = 64;
        tables(nc);
        if (big) {
            seq_t *nv = alloc_zero(nc), *nw = alloc_zero(nc);
            for (i = 0; i < done; i++) { nv[i] = V[i]; nw[i] = W[i]; }
            if (V) { release(V, cap); release(W, cap); }
            V = nv; W = nw;
        } else {
            V2 = (unsigned __int128 *)realloc(V2, (size_t)nc * sizeof(unsigned __int128));
            W2 = (unsigned __int128 *)realloc(W2, (size_t)nc * sizeof(unsigned __int128));
        }
        cap = nc;
    }
    for (i = done; i < need; i++) {
        if (big) {
            if (i == 0) { V[0] = 1; W[0] = 1; continue; }
            {
                seq_t pos = 0, neg = 0;
                for (s = 1; s * s <= i; s++) {
                    idx_t c = 2 * (7 * s * s + i);
                    if (s & 1) pos += V[i - s * s] * c;
                    else neg += V[i - s * s] * c;
                }
                V[i] = (pos - neg) / i;
            }
            {
                seq_t pos = 0, neg = 0;
                idx_t k, t;
                for (k = 1; (t = k * (k + 1) / 2) <= i; k++) {
                    idx_t c = 7 * t + i;
                    if (t & 1) pos += W[i - t] * c;
                    else neg += W[i - t] * c;
                }
                W[i] = (pos - neg) / i;
            }
        } else {
            unsigned __int128 av = 0, aw = 0;
            if (i == 0) { V2[0] = 1; W2[0] = 1; continue; }
            for (j = 1; j <= i; j++) {
                unsigned __int128 rv = (unsigned __int128)(unsigned long long)R8[j] * V2[i - j];
                unsigned __int128 rw = (unsigned __int128)(unsigned long long)P8[j] * W2[i - j];
                if (j & 1) { av += rv; aw += rw; }
                else { av -= rv; aw -= rw; }
            }
            V2[i] = av;
            W2[i] = aw;
        }
    }
    if (need > done) done = need;
}

seq_t a(idx_t n)
{
    idx_t N = n + 1, M = n - 1, j;
    if (n < -1) return 0;
    ensure(N + 1);
    if (seq_is_big()) {
        seq_t d = 0, res = 0;
        for (j = 0; j <= N; j++) d += W[N - j] * (idx_t)R8[j];
        if (n >= 1) {
            seq_t e = 0;
            for (j = 0; j <= M; j++) e += V[M - j] * (idx_t)P8[j];
            res += e * 4096;
        }
        if (n == 0) res += 128;
        if (N & 1) res -= d;
        else res += d;
        return res;
    } else {
        unsigned __int128 d = 0, res = 0;
        for (j = 0; j <= N; j++) d += W2[N - j] * (unsigned __int128)(unsigned long long)R8[j];
        if (n >= 1) {
            unsigned __int128 e = 0;
            for (j = 0; j <= M; j++) e += V2[M - j] * (unsigned __int128)(unsigned long long)P8[j];
            res += e * 4096;
        }
        if (n == 0) res += 128;
        if (N & 1) res -= d;
        else res += d;
        return u128_to_seq(res);
    }
}
