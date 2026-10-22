/*
 * A002955  Number of (unordered, unlabeled) rooted trimmed trees with n
 *          nodes.
 * https://oeis.org/A002955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(1) = 1, a(n+1) = EULER(c)(n) with c(k) = a(k) - [k=2]: with
 *            c'(k) = Sum_{d|k} d*c(d) and b(0) = 1, b(m) = (1/m) Sum_{k=1..m}
 *            c'(k)*b(m-k), a(m+1) = b(m)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 29, u64 n <= 53, u128 n <= 101,
 *            big any n
 * Helpers:   A_grow, a2955_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A_grow.h"
#include "../utils/a2955_is_fixed.h"

typedef struct { uint64_t w[4]; } a2955_u256;

static a2955_u256 a2955_zero(void) {
    a2955_u256 r;
    r.w[0] = r.w[1] = r.w[2] = r.w[3] = 0;
    return r;
}

static void a2955_add(a2955_u256 *x, const a2955_u256 *y) {
    unsigned __int128 carry = 0;
    int i;
    for (i = 0; i < 4; i++) {
        unsigned __int128 t = (unsigned __int128)x->w[i] + y->w[i] + carry;
        x->w[i] = (uint64_t)t;
        carry = t >> 64;
    }
}

static a2955_u256 a2955_mul(const a2955_u256 *x, const a2955_u256 *y) {
    a2955_u256 r = a2955_zero();
    int i, j;
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)x->w[i] * y->w[j] + r.w[i + j] + carry;
            r.w[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    return r;
}

static a2955_u256 a2955_muls(const a2955_u256 *x, uint64_t m) {
    a2955_u256 r;
    unsigned __int128 carry = 0;
    int i;
    for (i = 0; i < 4; i++) {
        unsigned __int128 t = (unsigned __int128)x->w[i] * m + carry;
        r.w[i] = (uint64_t)t;
        carry = t >> 64;
    }
    return r;
}

static void a2955_divs(a2955_u256 *x, uint64_t d) {
    unsigned __int128 rem = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | x->w[i];
        x->w[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
}

/* fixed-width path: exact 256-bit arithmetic */
static a2955_u256 *fB = NULL, *fC = NULL;
static idx_t fcnt = 0, fcap = 0;

static seq_t a2955_fixed_val(idx_t m) {
    if (fcnt == 0) {
        fcap = 64;
        fB = (a2955_u256 *)malloc(fcap * sizeof(a2955_u256));
        fC = (a2955_u256 *)malloc(fcap * sizeof(a2955_u256));
        fB[0] = a2955_zero();
        fB[0].w[0] = 1;
        fC[0] = a2955_zero();
        fcnt = 1;
    }
    if (m >= fcap) {
        idx_t nc = fcap;
        while (nc <= m) nc *= 2;
        fB = (a2955_u256 *)realloc(fB, nc * sizeof(a2955_u256));
        fC = (a2955_u256 *)realloc(fC, nc * sizeof(a2955_u256));
        fcap = nc;
    }
    while (fcnt <= m) {
        idx_t mm = fcnt, d, k;
        a2955_u256 cs = a2955_zero(), s = a2955_zero();
        for (d = 1; d * d <= mm; d++) {
            if (mm % d == 0) {
                idx_t e = mm / d;
                if (d != 2) { a2955_u256 t = a2955_muls(&fB[d - 1], d); a2955_add(&cs, &t); }
                if (e != d && e != 2) { a2955_u256 t = a2955_muls(&fB[e - 1], e); a2955_add(&cs, &t); }
            }
        }
        fC[mm] = cs;
        for (k = 1; k <= mm; k++) {
            a2955_u256 t = a2955_mul(&fC[k], &fB[mm - k]);
            a2955_add(&s, &t);
        }
        a2955_divs(&s, mm);
        fB[mm] = s;
        fcnt++;
    }
    {
        seq_t r = (seq_t)fB[m].w[1];
        int i;
        for (i = 0; i < 4; i++) r <<= 16;
        r += (seq_t)fB[m].w[0];
        return r;
    }
}

/* arbitrary-precision path */
static seq_t *gB = NULL, *gC = NULL;
static idx_t gcnt = 0, gcap = 0;

static seq_t a2955_big_val(idx_t m) {
    if (gcnt == 0) {
        gB = A_grow(NULL, 0, 64);
        gC = A_grow(NULL, 0, 64);
        gcap = 64;
        gB[0] = 1;
        gC[0] = 0;
        gcnt = 1;
    }
    if (m >= gcap) {
        idx_t nc = gcap;
        while (nc <= m) nc *= 2;
        gB = A_grow(gB, gcap, nc);
        gC = A_grow(gC, gcap, nc);
        gcap = nc;
    }
    while (gcnt <= m) {
        idx_t mm = gcnt, d, k;
        seq_t cs = 0, s = 0;
        for (d = 1; d * d <= mm; d++) {
            if (mm % d == 0) {
                idx_t e = mm / d;
                if (d != 2) cs += gB[d - 1] * (seq_t)d;
                if (e != d && e != 2) cs += gB[e - 1] * (seq_t)e;
            }
        }
        gC[mm] = cs;
        for (k = 1; k <= mm; k++) s += gC[k] * gB[mm - k];
        gB[mm] = s / (seq_t)mm;
        gcnt++;
    }
    return gB[m];
}

seq_t a(idx_t n) {
    static int mode = -1;
    if (n < 1) return 0;
    if (mode < 0) mode = a2955_is_fixed();
    if (mode) return a2955_fixed_val(n - 1);
    return a2955_big_val(n - 1);
}
