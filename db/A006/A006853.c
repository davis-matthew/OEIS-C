/*
 * A006853  Balanced colorings of n-cube.
 * https://oeis.org/A006853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   a(0) = 2; for n > 0, a(n) = N(n,2^(n-1)) +
 *            2*Sum_{k=0..2^(n-1)/2-1, 2k<2^(n-1)} N(n,2k), where N(n,m) =
 *            Sum_{partitions (j) of m} N((j))^n * (-1)^(m - #parts) / z((j)),
 *            N((j)) = [x^(m/2)] Product_i (1+x^i)^j[i], z((j)) = Product_i
 *            i^j[i] * j[i]!. (The k=0 term counts the empty set and, by
 *            complem...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   mulmod_s, powmod_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mulmod_s.h"
#include "../utils/powmod_s.h"

#define MAXP 4

static int G_np, G_n, G_half, G_K;
static uint64_t G_p[MAXP], G_pinv[MAXP], G_r2[MAXP], G_one[MAXP], G_acc[MAXP];
static uint64_t G_inv[MAXP][65];
static uint64_t G_poly[70][34];

static bool mr_isprime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, r, s = 0;
    uint64_t d = n - 1;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powmod_s(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulmod_s(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

static inline uint64_t mmul(uint64_t a, uint64_t b, int q) {
    unsigned __int128 t = (unsigned __int128)a * b;
    uint64_t m = (uint64_t)t * G_pinv[q];
    uint64_t r = (uint64_t)((t + (unsigned __int128)m * G_p[q]) >> 64);
    if (r >= G_p[q]) r -= G_p[q];
    return r;
}

static void dfs(int depth, int s, int last, int lastcnt, int parts, const uint64_t *W) {
    int q, i, d;
    uint64_t W2[MAXP];
    if ((s & 1) == 0) {
        uint64_t N = G_poly[depth][s >> 1];
        if (N) {
            int neg = (s - parts) & 1;
            for (q = 0; q < G_np; q++) {
                uint64_t p = G_p[q];
                uint64_t b = mmul(N % p, G_r2[q], q);
                uint64_t pw = b, c;
                int e;
                for (e = 1; e < G_n; e++) pw = mmul(pw, b, q);
                c = mmul(pw, W[q], q);
                if (s != G_half) { c += c; if (c >= p) c -= p; }
                if (neg) G_acc[q] = (G_acc[q] >= c) ? G_acc[q] - c : G_acc[q] + p - c;
                else { G_acc[q] += c; if (G_acc[q] >= p) G_acc[q] -= p; }
            }
        }
    }
    {
        int top = (last < G_half - s) ? last : G_half - s;
        for (i = top; i >= 1; i--) {
            int jn = (i == last) ? lastcnt + 1 : 1;
            uint64_t *src = G_poly[depth], *dst = G_poly[depth + 1];
            memcpy(dst, src, (size_t)(G_K + 1) * sizeof(uint64_t));
            for (d = G_K; d >= i; d--) dst[d] += dst[d - i];
            for (q = 0; q < G_np; q++) W2[q] = mmul(W[q], G_inv[q][i * jn], q);
            dfs(depth + 1, s + i, i, jn, parts + 1, W2);
        }
    }
}

seq_t a(idx_t n) {
    int q, i, j, k;
    uint64_t c, W[MAXP], dig[MAXP];
    seq_t x;
    if (n == 0) return (seq_t)2;
    if (n > 7) return (seq_t)0; /* partitions of 2^(n-1) become infeasible */

    G_n = (int)n;
    G_half = 1 << (n - 1);
    G_K = G_half / 2;
    G_np = (int)((1u << n) / 61 + 1);

    c = (1ULL << 62) - 1;
    for (q = 0; q < G_np; ) {
        if (mr_isprime(c)) G_p[q++] = c;
        c -= 2;
    }
    for (q = 0; q < G_np; q++) {
        uint64_t p = G_p[q], inv = p, rm;
        for (k = 0; k < 6; k++) inv *= 2 - p * inv;
        G_pinv[q] = (uint64_t)0 - inv;
        rm = (uint64_t)(((unsigned __int128)1 << 64) % p);
        G_one[q] = rm;
        G_r2[q] = mulmod_s(rm, rm, p);
        G_inv[q][0] = 0;
        for (i = 1; i <= 64; i++)
            G_inv[q][i] = mmul(powmod_s((uint64_t)i, p - 2, p), G_r2[q], q);
        G_acc[q] = 0;
        W[q] = G_one[q];
    }

    memset(G_poly[0], 0, sizeof(G_poly[0]));
    G_poly[0][0] = 1;
    dfs(0, 0, G_half, 0, 0, W);

    /* convert out of Montgomery form, then Garner CRT */
    for (q = 0; q < G_np; q++) {
        uint64_t r = mmul(G_acc[q], 1, q);
        for (j = 0; j < q; j++) {
            uint64_t p = G_p[q];
            uint64_t dj = dig[j] % p;
            uint64_t invpj = powmod_s(G_p[j] % p, p - 2, p);
            r = (r >= dj) ? r - dj : r + p - dj;
            r = mulmod_s(r, invpj, p);
        }
        dig[q] = r;
    }
    x = (seq_t)dig[G_np - 1];
    for (q = G_np - 2; q >= 0; q--) {
        x = x * (seq_t)G_p[q] + (seq_t)dig[q];
    }
    return x;
}
