/*
 * A001150  Number of n-input 2-output switching networks with GL(n,2) acting
 *          on the input and S(2) and C(2,2) acting on the output.
 * https://oeis.org/A001150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over GL(n,2) x D4: a(n) = (1/(8*|GL(n,2)|)) * Sum_{g in
 *            GL(n,2)} (4^c(g) + 2*2^o(g)*4^e(g)), where c, o, e = numbers of
 *            all / odd-length / even-length cycles of g on GF(2)^n (the other
 *            output symmetries fix nothing because 0 is a fixed point);
 *            summed over conjugacy classes (rational cano...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   q1150_add_shifted, q1150_div_small, q1150_gcd, q1150_mul_small,
 *            q1150_pdeg, q1150_pmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q1150_add_shifted.h"
#include "../utils/q1150_div_small.h"
#include "../utils/q1150_gcd.h"
#include "../utils/q1150_mul_small.h"
#include "../utils/q1150_pdeg.h"
#include "../utils/q1150_pmod.h"

typedef struct { int deg; uint64_t ord; uint64_t mask; } q1150_poly;

static int q1150_n;
static q1150_poly *q1150_P;
static int q1150_np;
static int q1150_cd[64];
static uint64_t q1150_ce[64];
static int q1150_cl[64][64];
static int q1150_clen[64];
static int q1150_nc;
static uint32_t *q1150_tot;
static int q1150_L;
static uint32_t *q1150_godd;
static uint32_t *q1150_cs;
static int q1150_GL;
static uint64_t q1150_dv[16384];
static uint64_t q1150_pp[16384];

static void q1150_leaf(void) {
    int n = q1150_n;
    long long cexp = 0, pexp;
    uint64_t N = 1;
    int nd = 0, nbig = 0, i, j, ci;
    uint64_t r;
    long long c = 0, o = 0;
    static uint64_t bigd[16384];

    memcpy(q1150_cs, q1150_godd, sizeof(uint32_t) * (size_t)q1150_GL);
    for (ci = 0; ci < q1150_nc; ci++) {
        int d = q1150_cd[ci], len = q1150_clen[ci];
        const int *lam = q1150_cl[ci];
        long long sconj = 0, sm2 = 0;
        int s = 0;
        uint64_t uo;
        for (j = 1; j <= lam[0]; j++) {
            long long cnt = 0;
            for (i = 0; i < len; i++) if (lam[i] >= j) cnt++;
            sconj += cnt * cnt;
        }
        i = 0;
        while (i < len) {
            int m = 0, t;
            int v = lam[i];
            while (i < len && lam[i] == v) { m++; i++; }
            sm2 += (long long)m * m;
            cexp += (long long)d * ((long long)m * (m - 1) / 2);
            for (t = 1; t <= m; t++)
                q1150_div_small(q1150_cs, q1150_GL, (uint32_t)((1ULL << (d * t)) - 1));
        }
        cexp += (long long)d * (sconj - sm2);
        while ((1 << s) < lam[0]) s++;
        uo = q1150_ce[ci] << s;
        N = N / q1150_gcd(N, uo) * uo;
    }
    pexp = (long long)n * (n - 1) / 2 - cexp;

    for (r = 1; r * r <= N; r++) {
        if (N % r == 0) {
            q1150_dv[nd++] = r;
            if (r != N / r) bigd[nbig++] = N / r;
        }
    }
    for (i = nbig - 1; i >= 0; i--) q1150_dv[nd++] = bigd[i];

    for (i = 0; i < nd; i++) {
        uint64_t k = q1150_dv[i];
        int s = 0;
        uint64_t m = k;
        long long pw, dim = 0;
        uint64_t F, Pk;
        while ((m & 1) == 0) { m >>= 1; s++; }
        pw = s >= 40 ? (1LL << 40) : (1LL << s);
        for (ci = 0; ci < q1150_nc; ci++) {
            if (m % q1150_ce[ci] == 0) {
                for (j = 0; j < q1150_clen[ci]; j++) {
                    long long p = q1150_cl[ci][j];
                    dim += (long long)q1150_cd[ci] * (p < pw ? p : pw);
                }
            }
        }
        F = 1ULL << dim;
        Pk = F;
        for (j = 0; j < i; j++)
            if (k % q1150_dv[j] == 0) Pk -= q1150_pp[j];
        q1150_pp[i] = Pk;
        c += (long long)(Pk / k);
        if (k & 1) o += (long long)(Pk / k);
    }

    q1150_add_shifted(q1150_tot, q1150_L, q1150_cs, q1150_GL, pexp + 2 * c);
    q1150_add_shifted(q1150_tot, q1150_L, q1150_cs, q1150_GL, pexp + 1 + o + 2 * (c - o));
}

static void q1150_rec(int i, int r);

static void q1150_part(int j, int r, int rem, int maxp, int len) {
    int p;
    if (rem == 0) {
        q1150_clen[q1150_nc] = len;
        q1150_nc++;
        q1150_rec(j + 1, r);
        q1150_nc--;
        return;
    }
    for (p = rem < maxp ? rem : maxp; p >= 1; p--) {
        q1150_cl[q1150_nc][len] = p;
        q1150_part(j, r, rem - p, p, len + 1);
    }
}

static void q1150_rec(int i, int r) {
    int j, m;
    if (r == 0) { q1150_leaf(); return; }
    for (j = i; j < q1150_np && q1150_P[j].deg <= r; j++) {
        int d = q1150_P[j].deg;
        q1150_cd[q1150_nc] = d;
        q1150_ce[q1150_nc] = q1150_P[j].ord;
        for (m = 1; m * d <= r; m++)
            q1150_part(j, r - m * d, m, m, 0);
    }
}

seq_t a(idx_t n_) {
    int n = (int)n_;
    int cap = 16, d, i, top, word, bit;
    long long S;
    seq_t res = 0;

    q1150_n = n;
    q1150_np = 0;
    q1150_P = (q1150_poly *)malloc(sizeof(q1150_poly) * (size_t)cap);
    for (d = 1; d <= n; d++) {
        uint64_t f;
        for (f = (1ULL << d) | 1ULL; f < (1ULL << (d + 1)); f += 2) {
            int irr = 1;
            for (i = 0; i < q1150_np && 2 * q1150_P[i].deg <= d; i++) {
                if (q1150_pmod(f, q1150_P[i].mask) == 0) { irr = 0; break; }
            }
            if (irr) {
                uint64_t rr = 1, k = 0;
                do {
                    rr <<= 1;
                    if (rr & (1ULL << d)) rr ^= f;
                    k++;
                } while (rr != 1);
                if (q1150_np == cap) {
                    cap *= 2;
                    q1150_P = (q1150_poly *)realloc(q1150_P, sizeof(q1150_poly) * (size_t)cap);
                }
                q1150_P[q1150_np].deg = d;
                q1150_P[q1150_np].ord = k;
                q1150_P[q1150_np].mask = f;
                q1150_np++;
            }
        }
    }

    q1150_GL = (n * n) / 32 + 3;
    q1150_godd = (uint32_t *)calloc((size_t)q1150_GL, sizeof(uint32_t));
    q1150_cs = (uint32_t *)calloc((size_t)q1150_GL, sizeof(uint32_t));
    q1150_godd[0] = 1;
    for (i = 1; i <= n; i++) q1150_mul_small(q1150_godd, q1150_GL, (uint32_t)((1ULL << i) - 1));

    q1150_L = (int)(((1LL << (n + 1)) + 2LL * n * n) / 32 + 8);
    q1150_tot = (uint32_t *)calloc((size_t)q1150_L, sizeof(uint32_t));
    q1150_nc = 0;

    q1150_rec(0, n);

    for (i = 1; i <= n; i++) q1150_div_small(q1150_tot, q1150_L, (uint32_t)((1ULL << i) - 1));
    S = 3 + (long long)n * (n - 1) / 2;
    word = (int)(S / 32);
    bit = (int)(S % 32);
    for (i = 0; i < q1150_L; i++) {
        uint32_t lo = i + word < q1150_L ? q1150_tot[i + word] : 0;
        uint32_t hi = i + word + 1 < q1150_L ? q1150_tot[i + word + 1] : 0;
        q1150_tot[i] = bit ? ((lo >> bit) | (hi << (32 - bit))) : lo;
    }

    top = q1150_L - 1;
    while (top > 0 && q1150_tot[top] == 0) top--;
    for (i = top; i >= 0; i--) {
        int b;
        for (b = 3; b >= 0; b--) {
            res = res * 256;
            res = res + (seq_t)((q1150_tot[i] >> (8 * b)) & 255u);
        }
    }

    free(q1150_P);
    free(q1150_godd);
    free(q1150_cs);
    free(q1150_tot);
    return res;
}
