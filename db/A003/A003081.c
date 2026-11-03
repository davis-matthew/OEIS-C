/*
 * A003081  Number of triangular cacti with 2n+1 nodes (n triangles).
 * https://oeis.org/A003081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = b(2n+1), where B(x) = C(x) + (C(x^3) - C(x)^3)/3 and C(x)
 *            = x*exp(Sum_{k>=1} T(x^k)/k), T(x) = (C(x)^2 + C(x^2))/2 (C =
 *            g.f. of rooted triangular cacti, A003080(n) = c(2n+1))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 73,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t a3081_mul(seq_t a, seq_t b)
{
    seq_t r = a * (b * 1u);
    return r;
}

static seq_t a3081_inv(seq_t d)
{
    seq_t inv = d;
    int k;
    for (k = 0; k < 9; k++) {
        seq_t q = 2;
        q -= a3081_mul(d, inv);
        inv = a3081_mul(inv, q);
    }
    return inv;
}

/* C(t+l-1, l) mod 2^w */
static seq_t a3081_mc(seq_t t, idx_t l, int w)
{
    seq_t odd = 1, dodd = 1, r;
    long e = 0;
    idx_t i;
    for (i = 0; i < l; i++) {
        seq_t x = t + i;
        if (x == 0) { e += w; continue; }
        while ((x & 1) == 0) { x >>= 1; e++; }
        odd = a3081_mul(odd, x);
    }
    for (i = 1; i <= l; i++) {
        idx_t y = i;
        while ((y & 1) == 0) { y >>= 1; e--; }
        dodd = a3081_mul(dodd, (seq_t)y);
    }
    if (e >= w) return 0;
    r = a3081_mul(odd, a3081_inv(dodd));
    while (e-- > 0) r = r + r;
    return r;
}

static seq_t a3081_fixed(idx_t n, int w)
{
    idx_t K = n, k, m, l, i, j;
    seq_t *H, *mcv, cube = 0, D = 0, E, res;
    if (n > (idx_t)w + 10) return 0;
    H = mem_new(seq_t, (size_t)(K + 1));
    mcv = mem_new(seq_t, (size_t)(K + 2));
    for (i = 0; i <= K; i++) H[i] = 0;
    H[0] = 1;
    for (k = 1; k <= K; k++) {
        seq_t tau = 0;
        idx_t L;
        for (i = 0; 2 * i < k - 1; i++) tau += a3081_mul(H[i], H[k - 1 - i]);
        if ((k - 1) % 2 == 0) {
            seq_t uh = H[(k - 1) / 2];
            seq_t v = uh + 1;
            if ((uh & 1) == 0) tau += a3081_mul(uh >> 1, v);
            else tau += a3081_mul(uh, v >> 1);
        }
        if (tau == 0) continue;
        L = K / k;
        for (l = 1; l <= L; l++) mcv[l] = a3081_mc(tau, l, w);
        for (m = K; m >= k; m--) {
            seq_t s = 0;
            for (l = 1; l * k <= m; l++) s += a3081_mul(mcv[l], H[m - l * k]);
            H[m] += s;
        }
    }
    for (i = 0; i < n; i++) {
        seq_t s2 = 0;
        idx_t r = n - 1 - i;
        for (j = 0; j <= r; j++) s2 += a3081_mul(H[j], H[r - j]);
        cube += a3081_mul(H[i], s2);
    }
    if ((2 * n + 1) % 3 == 0) D = H[((2 * n + 1) / 3 - 1) / 2];
    E = cube - D;
    res = H[n] - a3081_mul(E, a3081_inv((seq_t)3));
    mem_free(H);
    mem_free(mcv);
    return res;
}

static int a3081_w(void)
{
    seq_t x = 1;
    int w = 0;
    while (w < 300 && x != 0) { x = x + x; w++; }
    return w;
}

static seq_t *BU = NULL, *BT = NULL, *BS2 = NULL, *BSG = NULL;
static idx_t bhave = 0, bcap = 0;

static void a3081_ensure(idx_t n)
{
    idx_t m, i, d;
    if (n < bhave) return;
    if (n + 1 > bcap) {
        idx_t nc = 2 * bcap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        BU = A_grow(BU, bcap, nc);
        BT = A_grow(BT, bcap, nc);
        BS2 = A_grow(BS2, bcap, nc);
        BSG = A_grow(BSG, bcap, nc);
        bcap = nc;
    }
    for (m = bhave; m <= n; m++) {
        if (m == 0) {
            BU[0] = 1; BT[0] = 0; BSG[0] = 0; BS2[0] = 1;
            continue;
        }
        {
            seq_t t = BS2[m - 1];
            if ((m - 1) % 2 == 0) t += BU[(m - 1) / 2];
            BT[m] = t / 2;
        }
        {
            seq_t sg = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    sg += BT[d] * d;
                    if (e != d) sg += BT[e] * e;
                }
            }
            BSG[m] = sg;
        }
        {
            seq_t g = 0;
            for (i = 1; i <= m; i++) g += BSG[i] * BU[m - i];
            BU[m] = g / m;
        }
        {
            seq_t s = 0;
            for (i = 0; 2 * i < m; i++) s += BU[i] * BU[m - i];
            s = s * 2;
            if (m % 2 == 0) s += BU[m / 2] * BU[m / 2];
            BS2[m] = s;
        }
    }
    bhave = n + 1;
}

seq_t a(idx_t n)
{
    int w = a3081_w();
    seq_t cube = 0, D = 0, E;
    idx_t i;
    if (w < 300) return a3081_fixed(n, w);
    a3081_ensure(n);
    for (i = 0; i < n; i++) cube += BU[i] * BS2[n - 1 - i];
    if ((2 * n + 1) % 3 == 0) D = BU[((2 * n + 1) / 3 - 1) / 2];
    E = cube - D;
    return BU[n] - E / 3;
}
