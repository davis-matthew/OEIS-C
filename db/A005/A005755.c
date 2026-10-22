/*
 * A005755  Number of identity matched trees with n nodes.
 * https://oeis.org/A005755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (E(n) - F(n))/2, where r(x) = Sum r(n) x^n = x *
 *            Product_{k>=1} (1+x^k)^(2*r(k)) (rooted identity trees, 2 edge
 *            colors), E(n) = r(n) - Sum_{i=1..n-1} r(i)*r(n-i) - [n
 *            even]*r(n/2) (identity edge-2-colored trees), F(n) = [x^n] x *
 *            Product_{k>=1} (1+x^(2k))^r(k) (such trees isomorphic to the...
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"

/* ---------- fixed-width path: arithmetic mod 2^256 ---------- */
typedef struct { uint64_t l[4]; } u256;

static u256 u_small(uint64_t v) {
    u256 z;
    z.l[0] = v; z.l[1] = 0; z.l[2] = 0; z.l[3] = 0;
    return z;
}

static u256 u_add(u256 x, u256 y) {
    u256 z;
    unsigned __int128 c = 0;
    int i;
    for (i = 0; i < 4; i++) {
        c += (unsigned __int128)x.l[i] + y.l[i];
        z.l[i] = (uint64_t)c;
        c >>= 64;
    }
    return z;
}

static u256 u_sub(u256 x, u256 y) {
    u256 z;
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < 4; i++) {
        uint64_t xi = x.l[i], yi = y.l[i];
        uint64_t d0 = xi - yi;
        uint64_t nb = (xi < yi) || (d0 < borrow);
        z.l[i] = d0 - borrow;
        borrow = nb;
    }
    return z;
}

static u256 u_mul(u256 x, u256 y) {
    uint64_t r[4] = {0, 0, 0, 0};
    u256 z;
    int i, j;
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)x.l[i] * y.l[j] + r[i + j] + carry;
            r[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    for (i = 0; i < 4; i++) z.l[i] = r[i];
    return z;
}

static u256 u_shr(u256 x, int v) {
    u256 z;
    int i;
    if (v == 0) return x;
    for (i = 0; i < 3; i++) z.l[i] = (x.l[i] >> v) | (x.l[i + 1] << (64 - v));
    z.l[3] = x.l[3] >> v;
    return z;
}

static u256 u_inv(uint64_t o) {
    u256 x = u_small(o), ou = u_small(o), two = u_small(2);
    int i;
    for (i = 0; i < 8; i++) x = u_mul(x, u_sub(two, u_mul(ou, x)));
    return x;
}

static u256 u_div(u256 s, uint64_t m) {
    int v = 0;
    while ((m & 1) == 0) { m >>= 1; v++; }
    return u_mul(u_shr(s, v), u_inv(m));
}

#define NF 200
static u256 FR[NF + 2], FAc[NF + 2], FB[NF + 2], Fb[NF + 2], Fbp[NF + 2], Fa[NF + 2];
static bool f_ready = false;

static u256 u_bsum(int m, const u256 *R, uint64_t mult) {
    u256 pos = u_small(0), neg = u_small(0);
    int d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            int e = m / d;
            u256 t = u_mul(u_small((uint64_t)d * mult), R[d]);
            if (e & 1) pos = u_add(pos, t); else neg = u_add(neg, t);
            if (e != d) {
                t = u_mul(u_small((uint64_t)e * mult), R[e]);
                if (d & 1) pos = u_add(pos, t); else neg = u_add(neg, t);
            }
        }
    }
    return u_sub(pos, neg);
}

static void fixed_init(void) {
    int m, j, k, i;
    FAc[0] = u_small(1);
    FR[1] = u_small(1);
    for (m = 1; m < NF; m++) {
        u256 s = u_small(0);
        Fb[m] = u_bsum(m, FR, 2);
        for (j = 1; j <= m; j++) s = u_add(s, u_mul(Fb[j], FAc[m - j]));
        FAc[m] = u_div(s, (uint64_t)m);
        FR[m + 1] = FAc[m];
    }
    FB[0] = u_small(1);
    for (m = 1; m <= NF / 2; m++) {
        u256 s = u_small(0);
        Fbp[m] = u_bsum(m, FR, 1);
        for (j = 1; j <= m; j++) s = u_add(s, u_mul(Fbp[j], FB[m - j]));
        FB[m] = u_div(s, (uint64_t)m);
    }
    for (k = 1; k <= NF; k++) {
        u256 s = u_small(0);
        for (i = 1; i < k; i++) s = u_add(s, u_mul(FR[i], FR[k - i]));
        if ((k & 1) == 0) s = u_add(s, FR[k / 2]);
        else s = u_add(s, FB[(k - 1) / 2]);
        Fa[k] = u_shr(u_sub(FR[k], s), 1);
    }
    f_ready = true;
}

/* ---------- arbitrary-precision path ---------- */
static seq_t *BR, *BA, *BBv, *Bb, *Bbp, *Bav;
static idx_t bcap = 0, nA = 0, nB = 0, nAv = 0;

static seq_t big_bsum(idx_t m, seq_t mult) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            idx_t e = m / d;
            seq_t t = (seq_t)d * BR[d] * mult;
            if (e & 1) pos += t; else neg += t;
            if (e != d) {
                t = (seq_t)e * BR[e] * mult;
                if (d & 1) pos += t; else neg += t;
            }
        }
    }
    return pos - neg;
}

static seq_t **const big_arrs[6] = { &BR, &BA, &BBv, &Bb, &Bbp, &Bav };

static seq_t big_a(idx_t n) {
    idx_t m, j, k, i, h;
    if (n + 2 > bcap) {
        idx_t nc = 2 * bcap;
        int q;
        if (nc < n + 2) nc = n + 2;
        for (q = 0; q < 6; q++) {
            if (bcap == 0) *big_arrs[q] = alloc_zero(nc);
            else *big_arrs[q] = grow_zero(*big_arrs[q], bcap, nc);
        }
        bcap = nc;
    }
    if (nA == 0) { BA[0] = 1; BR[1] = 1; nA = 1; }
    while (nA < n) {
        seq_t s = 0;
        m = nA;
        Bb[m] = big_bsum(m, 2);
        for (j = 1; j <= m; j++) s += Bb[j] * BA[m - j];
        BA[m] = s / (seq_t)m;
        BR[m + 1] = BA[m];
        nA++;
    }
    h = (n - 1) / 2;
    if (nB == 0) { BBv[0] = 1; nB = 1; }
    while (nB <= h) {
        seq_t s = 0;
        m = nB;
        Bbp[m] = big_bsum(m, 1);
        for (j = 1; j <= m; j++) s += Bbp[j] * BBv[m - j];
        BBv[m] = s / (seq_t)m;
        nB++;
    }
    for (k = nAv + 1; k <= n; k++) {
        seq_t s = 0;
        for (i = 1; i < k; i++) s += BR[i] * BR[k - i];
        if ((k & 1) == 0) s += BR[k / 2];
        else s += BBv[(k - 1) / 2];
        Bav[k] = (BR[k] - s) / 2;
    }
    if (n > nAv) nAv = n;
    return Bav[n];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (seq_is_big()) return big_a(n);
    if (n > NF) return 0; /* far beyond any fixed-width range */
    if (!f_ready) fixed_init();
    {
        seq_t r = (seq_t)Fa[n].l[1];
        r *= (seq_t)65536;
        r *= (seq_t)65536;
        r *= (seq_t)65536;
        r *= (seq_t)65536;
        r += (seq_t)Fa[n].l[0];
        return r;
    }
}
