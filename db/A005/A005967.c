/*
 * A005967  Number of isomorphism classes of connected 3-regular multigraphs
 *          of order 2n, loops allowed.
 * https://oeis.org/A005967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse Euler transform of A129427 (all 3-regular
 *            multigraphs with loops on 2n vertices). A129427(m) = [t^(2m)]
 *            E[exp(Sum_{L>=1} t^L/L * (X_L^3 + 3*X_L*X_{2L} + 2*X_{3L})/6)].
 *            This is Burnside's lemma for S_3 wr S_{2m} acting on half-edge
 *            pairings. The X_k are independent Gaussians N([k ev...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef unsigned long long u64;
typedef unsigned __int128 u128;

static u64 P;
static u64 mulm(u64 a, u64 b) { return (u64)((u128)a * b % P); }
static u64 addm(u64 a, u64 b) { u64 s = a + b; return s >= P ? s - P : s; }
static u64 powm(u64 b, u64 e) { u64 r = 1; b %= P; while (e) { if (e & 1) r = mulm(r, b); b = mulm(b, b); e >>= 1; } return r; }
static u64 invm(u64 a) { return powm(a % P, P - 2); }

static u64 mulmod64(u64 a, u64 b, u64 m) { return (u64)((u128)a * b % m); }
static u64 powmod64(u64 b, u64 e, u64 m) { u64 r = 1; b %= m; while (e) { if (e & 1) r = mulmod64(r, b, m); b = mulmod64(b, b, m); e >>= 1; } return r; }
static int isprime64(u64 n) {
    static const u64 bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    u64 d; int s = 0, i, r;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++) { if (n % bs[i] == 0) return n == bs[i]; }
    d = n - 1; while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        u64 x = powmod64(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) { x = mulmod64(x, x, n); if (x == n - 1) break; }
        if (r == s) return 0;
    }
    return 1;
}

static u64 prime_tab[1 << 14];
static int nprime = 0;
static u64 prime_cand = (1ULL << 60) - 1;
static u64 get_prime(int k) {
    while (nprime <= k) {
        while (!isprime64(prime_cand)) prime_cand -= 2;
        prime_tab[nprime++] = prime_cand;
        prime_cand -= 2;
    }
    return prime_tab[k];
}

static int mobius(int m) {
    int r = 1, q;
    for (q = 2; q * q <= m; q++) {
        if (m % q == 0) { m /= q; if (m % q == 0) return 0; r = -r; }
    }
    if (m > 1) r = -r;
    return r;
}

static u64 a_mod(int n) {
    int N = 2 * n, FM = 3 * N + 2, i, o, m;
    u64 *ifact = (u64 *)malloc(sizeof(u64) * (FM + 1));
    u64 *F = (u64 *)calloc(N + 1, sizeof(u64));
    u64 *b, *c, s, res;
    u64 f = 1;
    for (i = 1; i <= FM; i++) f = mulm(f, (u64)i);
    ifact[FM] = invm(f);
    for (i = FM; i > 0; i--) ifact[i - 1] = mulm(ifact[i], (u64)i);
    F[0] = 1;

    for (o = 1; o <= 3 * N; o += 2) {
        int w3 = (o % 3 == 0);
        int u, No, w, J, Dp, j, x;
        u64 **Gp, *H;
        if (!w3 && o > N) continue;
        u = w3 ? o / 3 : o;
        No = N / u;
        if (No == 0) continue;
        w = w3 ? 3 : 1;
        J = 0;
        while ((w << (J + 1)) <= 3 * No) J++;
        Dp = 0;
        Gp = (u64 **)malloc(sizeof(u64 *));
        Gp[0] = (u64 *)calloc(No + 1, sizeof(u64));
        Gp[0][0] = 1;
        for (j = J; j >= 0; j--) {
            int K = w << j;
            u64 kk = (u64)o << j;
            int halfK = K / 2;
            int emax = j > 0 ? No / halfK : 0;
            int mmax = Dp + emax;
            int dmax1 = w3 ? No / (K / 3) : 3 * (No / K);
            int dm = dmax1 + mmax;
            int bstep = w3 ? K / 3 : 0;
            int mu = j > 0, a, bb, e, d;
            u64 *M = (u64 *)malloc(sizeof(u64) * (dm + 2));
            u64 **R = (u64 **)malloc(sizeof(u64 *) * (mmax + 1));
            u64 **Gn = (u64 **)malloc(sizeof(u64 *) * (emax + 1));
            u64 i6k = invm(mulm(6, kk % P)), ik = invm(kk % P), ca, cepow;
            M[0] = 1;
            M[1] = (u64)mu;
            for (d = 1; d < dm; d++) {
                u64 t = mulm(((u64)d * kk) % P, M[d - 1]);
                M[d + 1] = addm(mu ? M[d] : 0, t);
            }
            for (m = 0; m <= mmax; m++) R[m] = (u64 *)calloc(No + 1, sizeof(u64));
            ca = 1;
            for (a = 0; a * K <= No; a++) {
                u64 cab = mulm(ca, ifact[a]);
                for (bb = 0;; bb++) {
                    int ex = a * K + bb * bstep;
                    u64 cc;
                    if (ex > No) break;
                    cc = mulm(cab, ifact[bb]);
                    for (m = 0; m <= mmax; m++)
                        R[m][ex] = addm(R[m][ex], mulm(cc, M[3 * a + bb + m]));
                    if (!w3) break;
                    cab = mulm(cab, ik);
                }
                ca = mulm(ca, i6k);
            }
            cepow = 1;
            for (e = 0; e <= emax; e++) {
                int shift = e * halfK, lim = No - shift, z, d2;
                u64 ce = mulm(cepow, ifact[e]);
                Gn[e] = (u64 *)calloc(No + 1, sizeof(u64));
                for (z = 0; z <= lim; z++) {
                    u128 acc = 0;
                    int cnt = 0;
                    for (d2 = 0; d2 <= Dp; d2++) {
                        int x0 = d2 * K, xx;
                        const u64 *g = Gp[d2], *r = R[d2 + e];
                        if (x0 > z) break;
                        for (xx = x0; xx <= z; xx++) {
                            acc += (u128)g[xx] * r[z - xx];
                            if (++cnt == 255) { acc %= P; cnt = 0; }
                        }
                    }
                    Gn[e][shift + z] = mulm((u64)(acc % P), ce);
                }
                cepow = mulm(cepow, ik);
            }
            for (m = 0; m <= Dp; m++) free(Gp[m]);
            free(Gp);
            for (m = 0; m <= mmax; m++) free(R[m]);
            free(R);
            free(M);
            Gp = Gn;
            Dp = emax;
        }
        H = Gp[0];
        for (x = N; x >= 0; x--) {
            u64 acc = 0;
            int ii;
            for (ii = 0; ii <= No && u * ii <= x; ii++)
                if (H[ii]) acc = addm(acc, mulm(H[ii], F[x - u * ii]));
            F[x] = acc;
        }
        for (m = 0; m <= Dp; m++) free(Gp[m]);
        free(Gp);
    }

    b = (u64 *)calloc(n + 1, sizeof(u64));
    c = (u64 *)calloc(n + 1, sizeof(u64));
    for (m = 1; m <= n; m++) b[m] = F[2 * m];
    for (m = 1; m <= n; m++) {
        u64 t = mulm((u64)m, b[m]);
        for (i = 1; i < m; i++) t = (t + P - mulm(c[i], b[m - i])) % P;
        c[m] = t;
    }
    s = 0;
    for (i = 1; i <= n; i++) {
        if (n % i == 0) {
            int mb = mobius(n / i);
            if (mb == 1) s = addm(s, c[i]);
            else if (mb == -1) s = (s + P - c[i]) % P;
        }
    }
    res = mulm(s, invm((u64)n));
    free(b); free(c); free(F); free(ifact);
    return res;
}

seq_t a(idx_t n) {
    int nn = (int)n, k, i, cap = 64;
    u64 *dig = (u64 *)malloc(sizeof(u64) * cap);
    seq_t res = 0, mult = 1;
    for (k = 0;; k++) {
        u64 p = get_prime(k), r, v = 0, prod = 1;
        P = p;
        r = a_mod(nn);
        for (i = 0; i < k; i++) {
            v = (v + mulm(dig[i] % p, prod)) % p;
            prod = mulm(prod, prime_tab[i] % p);
        }
        if (k >= cap) { cap *= 2; dig = (u64 *)realloc(dig, sizeof(u64) * cap); }
        dig[k] = mulm((r + p - v) % p, invm(prod));
        if (k >= 2 && dig[k] == 0 && dig[k - 1] == 0) break;
    }
    for (i = 0; i <= k; i++) {
        res += (seq_t)dig[i] * mult;
        mult *= (seq_t)prime_tab[i];
    }
    free(dig);
    return res;
}
