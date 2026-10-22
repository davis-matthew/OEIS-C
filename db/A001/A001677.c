/*
 * A001677  Number of series-parallel networks with n edges.
 * https://oeis.org/A001677
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = s(n) - Sum_{1<=i<n-i} s(i)*s(n-i) - [n
 *            even]*s(n/2)*(s(n/2)+1)/2, where s = A000084. s(1) = 1, s(n) =
 *            2*b(n) for n >= 2, b = A000669 (b(1) = 1), computed by the Euler
 *            transform prod_k (1-x^k)^(-b(k)) = 1 + 2*B(x) - x, i.e. n*b(n) =
 *            Sum_{j|n, j<n} j*b(j) + Sum_{k=1..n-1} d(k)*s(n-k), d(k) =...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 43, u128 n <= 79,
 *            big any n
 * Helpers:   sp_is_fixed, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/zalloc_arr.h"
#include "../utils/sp_is_fixed.h"

/* ---------- arbitrary-precision path ---------- */
static seq_t *gB = NULL, *gC = NULL, *gD = NULL;
static idx_t gN = 0, gCap = 0;

static void big_ensure(idx_t n) {
    idx_t m, k, j, i;
    if (gC != NULL && n <= gN) return;
    if (n + 1 > gCap) {
        idx_t nc = gCap * 2;
        seq_t *nb, *ncc, *nd;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        nb = zalloc_arr(nc);
        ncc = zalloc_arr(nc);
        nd = zalloc_arr(nc);
        if (gC != NULL) {
            for (i = 0; i <= gN; i++) {
                nb[i] = gB[i];
                ncc[i] = gC[i];
                nd[i] = gD[i];
            }
        }
        gB = nb; gC = ncc; gD = nd; gCap = nc;
    }
    if (gN == 0) {
        gC[0] = 1; gB[1] = 1; gC[1] = 1; gD[1] = 1;
        gN = 1;
    }
    for (m = gN + 1; m <= n; m++) {
        seq_t R = 0, dpart, b;
        for (j = 1; j * j <= m; j++) {
            if (m % j == 0) {
                idx_t q = m / j;
                if (j < m) R += (seq_t)j * gB[j];
                if (q != j && q < m) R += (seq_t)q * gB[q];
            }
        }
        dpart = R;
        for (k = 1; k < m; k++) R += gD[k] * gC[m - k];
        b = R / (seq_t)m;
        gB[m] = b;
        gC[m] = b + b;
        gD[m] = dpart + (seq_t)m * b;
    }
    gN = n;
}

static seq_t big_a(idx_t n) {
    seq_t res;
    idx_t i;
    big_ensure(n);
    res = gC[n];
    for (i = 1; 2 * i < n; i++) res -= gC[i] * gC[n - i];
    if (n % 2 == 0) {
        seq_t h = gC[n / 2];
        res -= h * (h + 1) / 2;
    }
    return res;
}

/* ---------- fixed-width path: exact 256-bit arithmetic ---------- */
typedef struct { uint64_t w[4]; } sp_u;

static sp_u sp_small(uint64_t v) {
    sp_u r;
    memset(&r, 0, sizeof r);
    r.w[0] = v;
    return r;
}

static sp_u sp_add(sp_u a, sp_u b) {
    sp_u r;
    unsigned __int128 c = 0;
    int i;
    for (i = 0; i < 4; i++) {
        c += (unsigned __int128)a.w[i] + b.w[i];
        r.w[i] = (uint64_t)c;
        c >>= 64;
    }
    return r;
}

static sp_u sp_sub(sp_u a, sp_u b) {
    sp_u r;
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < 4; i++) {
        uint64_t x = a.w[i], y = b.w[i];
        uint64_t d = x - y - borrow;
        borrow = (x < y || (x == y && borrow)) ? 1 : 0;
        r.w[i] = d;
    }
    return r;
}

static sp_u sp_mul(sp_u a, sp_u b) {
    sp_u r;
    int i, j;
    memset(&r, 0, sizeof r);
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)a.w[i] * b.w[j] + r.w[i + j] + carry;
            r.w[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    return r;
}

static sp_u sp_divs(sp_u a, uint64_t d) {
    sp_u r;
    unsigned __int128 rem = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | a.w[i];
        r.w[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
    return r;
}

static sp_u *fB = NULL, *fC = NULL, *fD = NULL;
static idx_t fN = 0, fCap = 0;

static void fix_ensure(idx_t n) {
    idx_t m, k, j;
    if (fC != NULL && n <= fN) return;
    if (n + 1 > fCap) {
        idx_t nc = fCap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        fB = (sp_u *)realloc(fB, nc * sizeof(sp_u));
        fC = (sp_u *)realloc(fC, nc * sizeof(sp_u));
        fD = (sp_u *)realloc(fD, nc * sizeof(sp_u));
        fCap = nc;
    }
    if (fN == 0) {
        fC[0] = sp_small(1); fB[0] = sp_small(0); fD[0] = sp_small(0);
        fB[1] = sp_small(1); fC[1] = sp_small(1); fD[1] = sp_small(1);
        fN = 1;
    }
    for (m = fN + 1; m <= n; m++) {
        sp_u R = sp_small(0), dpart, b;
        for (j = 1; j * j <= m; j++) {
            if (m % j == 0) {
                idx_t q = m / j;
                if (j < m) R = sp_add(R, sp_mul(sp_small(j), fB[j]));
                if (q != j && q < m) R = sp_add(R, sp_mul(sp_small(q), fB[q]));
            }
        }
        dpart = R;
        for (k = 1; k < m; k++) R = sp_add(R, sp_mul(fD[k], fC[m - k]));
        b = sp_divs(R, m);
        fB[m] = b;
        fC[m] = sp_add(b, b);
        fD[m] = sp_add(dpart, sp_mul(sp_small(m), b));
    }
    fN = n;
}

static seq_t fix_a(idx_t n) {
    sp_u res;
    seq_t r = 0;
    idx_t i;
    int li, bi;
    fix_ensure(n);
    res = fC[n];
    for (i = 1; 2 * i < n; i++) res = sp_sub(res, sp_mul(fC[i], fC[n - i]));
    if (n % 2 == 0) {
        sp_u h = fC[n / 2];
        res = sp_sub(res, sp_divs(sp_mul(h, sp_add(h, sp_small(1))), 2));
    }
    for (li = 1; li >= 0; li--) {
        for (bi = 7; bi >= 0; bi--) {
            unsigned byte = (unsigned)((res.w[li] >> (8 * bi)) & 0xFF);
            r = (r << 8) + (seq_t)byte;
        }
    }
    return r;
}

seq_t a(idx_t n) {
    if (sp_is_fixed()) return fix_a(n);
    return big_a(n);
}
