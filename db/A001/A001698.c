/*
 * A001698  a(n) = ceiling(sqrt( 2*Pi )^n).
 * https://oeis.org/A001698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(sqrt(2*Pi)^n) = ceiling((2*Pi)^floor(n/2) *
 *            sqrt(2*Pi)^(n mod 2)), computed in exact fixed-point integer
 *            arithmetic: Pi = 16*atan(1/5) - 4*atan(1/239) (Machin),
 *            sqrt(2*Pi) by bitwise integer square root, power by repeated
 *            squaring
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 466 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a1698_add(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)x[i] + y[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a1698_addbit(uint32_t *x, size_t L, size_t p) {
    size_t i = p / 32;
    uint64_t c = (uint64_t)1 << (p % 32);
    for (; i < L && c; i++) {
        c += x[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a1698_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t rem = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static int a1698_iszero(const uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++)
        if (x[i]) return 0;
    return 1;
}

static void a1698_sub(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] - y[i] - br;
        x[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

/* sum += m * atan(1/x) in fixed point with fl fractional limbs */
static void a1698_atan(uint32_t *sum, size_t L, size_t fl, uint32_t x, uint32_t m) {
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t k;
    t[fl] = m;
    a1698_divs(t, L, x);
    for (k = 0; !a1698_iszero(t, L); k++) {
        memcpy(q, t, L * sizeof(uint32_t));
        a1698_divs(q, L, 2 * k + 1);
        if (k & 1) a1698_sub(sum, q, L);
        else a1698_add(sum, q, L);
        a1698_divs(t, L, x * x);
    }
    free(t);
    free(q);
}

static int a1698_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    size_t i;
    for (i = L; i-- > 0;) {
        if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
    }
    return 0;
}

/* fixed-point product (a*b) >> (32*fl); frees nothing */
static uint32_t *a1698_fmul(const uint32_t *x, size_t lx, const uint32_t *y, size_t ly,
                            size_t fl, size_t *lc) {
    size_t tot = lx + ly, i, j, m;
    uint32_t *c = (uint32_t *)calloc(tot, sizeof(uint32_t));
    for (i = 0; i < lx; i++) {
        uint64_t carry = 0;
        if (x[i] == 0) continue;
        for (j = 0; j < ly; j++) {
            uint64_t t = (uint64_t)x[i] * y[j] + c[i + j] + carry;
            c[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        c[i + ly] = (uint32_t)carry;
    }
    m = tot - fl;
    memmove(c, c + fl, m * sizeof(uint32_t));
    while (m > 1 && c[m - 1] == 0) m--;
    *lc = m;
    return c;
}

static void a1698_shr1(uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++) {
        uint32_t hi = (i + 1 < L) ? (x[i + 1] << 31) : 0;
        x[i] = (x[i] >> 1) | hi;
    }
}

/* floor(sqrt(N)), N has L limbs; result has L limbs */
static uint32_t *a1698_isqrt(const uint32_t *N, size_t L) {
    uint32_t *num = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)malloc(L * sizeof(uint32_t));
    size_t top = L * 32;
    long long p;
    memcpy(num, N, L * sizeof(uint32_t));
    while (top > 0 && !((num[(top - 1) / 32] >> ((top - 1) % 32)) & 1)) top--;
    if (top == 0) { free(num); free(tmp); return res; }
    p = (long long)((top - 1) & ~(size_t)1);
    for (; p >= 0; p -= 2) {
        memcpy(tmp, res, L * sizeof(uint32_t));
        a1698_addbit(tmp, L, (size_t)p);
        if (a1698_cmp(num, tmp, L) >= 0) {
            a1698_sub(num, tmp, L);
            a1698_shr1(res, L);
            a1698_addbit(res, L, (size_t)p);
        } else {
            a1698_shr1(res, L);
        }
    }
    free(num);
    free(tmp);
    return res;
}

seq_t a(idx_t n) {
    static uint32_t *cpi2 = NULL, *cs = NULL;
    static size_t cfl = 0;
    size_t B, fl, off, la, i;
    const uint32_t *pi2, *s;
    uint32_t *acc;
    idx_t k, bit;
    seq_t r;

    if (n == 0) return 1;
    B = (size_t)(n * 1326 / 1000) + 2;
    fl = (B + 160) / 32 + 1;

    if (fl > cfl) {
        size_t nfl = fl, L, NL;
        uint32_t *sa, *sb, *N, *root;
        if (cfl + cfl / 2 > nfl) nfl = cfl + cfl / 2;
        L = nfl + 1;
        sa = (uint32_t *)calloc(L, sizeof(uint32_t));
        sb = (uint32_t *)calloc(L, sizeof(uint32_t));
        a1698_atan(sa, L, nfl, 5, 16);
        a1698_atan(sb, L, nfl, 239, 4);
        a1698_sub(sa, sb, L);
        a1698_add(sa, sa, L); /* 2*Pi */
        free(sb);
        NL = 2 * nfl + 1;
        N = (uint32_t *)calloc(NL, sizeof(uint32_t));
        memcpy(N + nfl, sa, L * sizeof(uint32_t));
        root = a1698_isqrt(N, NL);
        free(N);
        free(cpi2);
        free(cs);
        cs = (uint32_t *)malloc(L * sizeof(uint32_t));
        memcpy(cs, root, L * sizeof(uint32_t));
        free(root);
        cpi2 = sa;
        cfl = nfl;
    }
    off = cfl - fl;
    pi2 = cpi2 + off;
    s = cs + off;

    acc = (uint32_t *)calloc(fl + 1, sizeof(uint32_t));
    acc[fl] = 1;
    la = fl + 1;
    k = n / 2;
    if (k > 0) {
        bit = 1;
        while (bit <= k / 2) bit <<= 1;
        for (; bit > 0; bit >>= 1) {
            size_t nl;
            uint32_t *t = a1698_fmul(acc, la, acc, la, fl, &nl);
            free(acc);
            acc = t;
            la = nl;
            if (k & bit) {
                t = a1698_fmul(acc, la, pi2, fl + 1, fl, &nl);
                free(acc);
                acc = t;
                la = nl;
            }
        }
    }
    if (n & 1) {
        size_t nl;
        uint32_t *t = a1698_fmul(acc, la, s, fl + 1, fl, &nl);
        free(acc);
        acc = t;
        la = nl;
    }

    r = 0;
    for (i = la; i-- > fl;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            r <<= 8;
            r += (acc[i] >> sh) & 255u;
        }
    }
    free(acc);
    r += 1;
    return r;
}
