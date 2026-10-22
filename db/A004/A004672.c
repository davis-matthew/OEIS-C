/*
 * A004672  Theta series of extremal even unimodular lattice in dimension 48.
 * https://oeis.org/A004672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = E4^6 - 1440*E4^3*Delta + 125280*Delta^2 = (E4^3 -
 *            720*Delta)^2 - 393120*Delta^2, with E4 = 1 + 240*Sum_{k>=1}
 *            sigma_3(k)*q^k, E4^3 = E4*E8, E8 = 1 + 480*Sum_{k>=1}
 *            sigma_7(k)*q^k, Delta = Sum_{m>=0} c(m)*q^(m+1), c(0) = 1,
 *            m*c(m) = -24*Sum_{k=1..m} sigma(k)*c(m-k)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 9, u128 n <= 65,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;
typedef __int128 i128_t;

#define BL 4   /* limbs for B = E4^3 - 720*Delta */
#define FL 10  /* limbs for the result */

static idx_t cap = 0, cnt = 0;
static uint64_t *e4c = NULL;  /* 240*sigma_3 */
static u128_t *e8c = NULL;    /* 480*sigma_7 */
static uint64_t *s1c = NULL;  /* sigma_1 */
static i128_t *etac = NULL;   /* coefficients of prod (1-q^k)^24 */
static uint64_t *bb = NULL;   /* B_n limbs */
static int *blen = NULL;
static uint64_t *ff = NULL;   /* a(n) limbs */

static u128_t sigp(uint64_t m, int p) {
    u128_t s = 0;
    for (uint64_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            u128_t t = 1, u = 1;
            for (int k = 0; k < p; k++) { t *= d; u *= e; }
            s += t;
            if (e != d) s += u;
        }
    }
    return s;
}

static int mulw(const uint64_t *x, int lx, const uint64_t *y, int ly, uint64_t *out) {
    memset(out, 0, sizeof(uint64_t) * (size_t)(lx + ly));
    for (int i = 0; i < lx; i++) {
        u128_t carry = 0;
        for (int j = 0; j < ly; j++) {
            u128_t t = (u128_t)x[i] * y[j] + out[i + j] + carry;
            out[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
        out[i + ly] = (uint64_t)carry;
    }
    return lx + ly;
}

static void addw(uint64_t *acc, int K, const uint64_t *t, int lt, int sub) {
    int i = 0;
    if (!sub) {
        u128_t c = 0;
        for (; i < lt && i < K; i++) {
            c += (u128_t)acc[i] + t[i];
            acc[i] = (uint64_t)c;
            c >>= 64;
        }
        for (; c && i < K; i++) {
            c += acc[i];
            acc[i] = (uint64_t)c;
            c >>= 64;
        }
    } else {
        uint64_t br = 0;
        for (; i < lt && i < K; i++) {
            uint64_t x = acc[i], y = t[i];
            uint64_t d = x - y;
            uint64_t nb = (x < y) | (d < br);
            acc[i] = d - br;
            br = nb;
        }
        for (; br && i < K; i++) {
            br = (acc[i] == 0);
            acc[i] -= 1;
        }
    }
}

static void dlimbs(i128_t v, uint64_t mul, uint64_t *out, int *neg) {
    u128_t mag = v < 0 ? -(u128_t)v : (u128_t)v;
    mag *= mul;
    out[0] = (uint64_t)mag;
    out[1] = (uint64_t)(mag >> 64);
    *neg = v < 0;
}

static void extend(idx_t n) {
    if (n < cnt) return;
    if (n >= cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        e4c = (uint64_t *)realloc(e4c, sizeof(uint64_t) * nc);
        e8c = (u128_t *)realloc(e8c, sizeof(u128_t) * nc);
        s1c = (uint64_t *)realloc(s1c, sizeof(uint64_t) * nc);
        etac = (i128_t *)realloc(etac, sizeof(i128_t) * nc);
        bb = (uint64_t *)realloc(bb, sizeof(uint64_t) * BL * nc);
        blen = (int *)realloc(blen, sizeof(int) * nc);
        ff = (uint64_t *)realloc(ff, sizeof(uint64_t) * FL * nc);
        cap = nc;
    }
    uint64_t tmp[2 * BL + 2], x[2], y[2];
    for (idx_t m = cnt; m <= n; m++) {
        if (m == 0) {
            e4c[0] = 1; e8c[0] = 1; s1c[0] = 0; etac[0] = 1;
        } else {
            e4c[m] = 240 * (uint64_t)sigp(m, 3);
            e8c[m] = 480 * sigp(m, 7);
            s1c[m] = (uint64_t)sigp(m, 1);
            i128_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += (i128_t)s1c[k] * etac[m - k];
            etac[m] = -24 * s / (i128_t)m;
        }
        /* B_m = (E4*E8)_m - 720*Delta_m */
        uint64_t *b = bb + m * BL;
        memset(b, 0, sizeof(uint64_t) * BL);
        for (idx_t i = 0; i <= m; i++) {
            uint64_t xv = e4c[i];
            y[0] = (uint64_t)e8c[m - i];
            y[1] = (uint64_t)(e8c[m - i] >> 64);
            int lt = mulw(&xv, 1, y, 2, tmp);
            addw(b, BL, tmp, lt, 0);
        }
        if (m >= 1) {
            int neg;
            dlimbs(etac[m - 1], 720, x, &neg);
            addw(b, BL, x, 2, !neg);
        }
        int l = BL;
        while (l > 0 && b[l - 1] == 0) l--;
        blen[m] = l;
        /* a(m) = Sum_{i+j=m} (B_i*B_j - 393120*Delta_i*Delta_j) */
        uint64_t *acc = ff + m * FL;
        memset(acc, 0, sizeof(uint64_t) * FL);
        for (idx_t i = 0; 2 * i <= m; i++) {
            idx_t j = m - i;
            int rep = (i < j) ? 2 : 1;
            if (blen[i] > 0 && blen[j] > 0) {
                int lt = mulw(bb + i * BL, blen[i], bb + j * BL, blen[j], tmp);
                for (int r = 0; r < rep; r++) addw(acc, FL, tmp, lt, 0);
            }
            if (i >= 1) {
                int ni, nj;
                dlimbs(etac[i - 1], 393120, x, &ni);
                dlimbs(etac[j - 1], 1, y, &nj);
                int lt = mulw(x, 2, y, 2, tmp);
                for (int r = 0; r < rep; r++) addw(acc, FL, tmp, lt, ni == nj);
            }
        }
    }
    cnt = n + 1;
}

seq_t a(idx_t n) {
    extend(n);
    const uint64_t *f = ff + n * FL;
    int k = FL - 1;
    while (k > 0 && f[k] == 0) k--;
    seq_t v = u128_to_seq((u128_t)f[k]);
    for (k--; k >= 0; k--) {
        for (int t = 0; t < 8; t++) v *= 256;
        v += u128_to_seq((u128_t)f[k]);
    }
    return v;
}
