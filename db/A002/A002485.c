/*
 * A002485  Numerators of convergents to Pi.
 * https://oeis.org/A002485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = c(n-2)*a(n-1) + a(n-2), a(0) = 0, a(1) = 1, where c(k)
 *            are the partial quotients of the continued fraction of Pi
 *            (A001203), computed exactly from rigorous lower/upper bounds on
 *            Pi (Machin's formula) via Euclid's algorithm
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 19, u64 n <= 34, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1902 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void pc_add(uint32_t *a, const uint32_t *b, size_t n, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        c += (uint64_t)a[i] + b[i];
        a[i] = (uint32_t)c;
        c >>= 32;
    }
    for (; i < L && c; i++) {
        c += a[i];
        a[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void pc_divs(uint32_t *x, size_t n, uint32_t d) {
    uint64_t r = 0;
    size_t i = n;
    while (i-- > 0) {
        uint64_t c = (r << 32) | x[i];
        x[i] = (uint32_t)(c / d);
        r = c % d;
    }
}

static void pc_sub(uint32_t *a, const uint32_t *b, size_t n, size_t L) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t ai = a[i], s = (uint64_t)b[i] + br;
        a[i] = (uint32_t)(ai - s);
        br = ai < s ? 1 : 0;
    }
    for (; i < L && br; i++) {
        uint64_t ai = a[i];
        a[i] = (uint32_t)(ai - 1);
        br = ai < 1 ? 1 : 0;
    }
}

/* sum = atan(1/x) as fixed point with FL fraction limbs */
static void pc_atan_inv(uint32_t *sum, size_t L, size_t FL, uint32_t x,
                        uint32_t *t, uint32_t *w) {
    size_t tn = FL + 1;
    uint32_t x2 = x * x;
    uint32_t k;
    memset(t, 0, L * sizeof(uint32_t));
    t[FL] = 1;
    pc_divs(t, tn, x);
    memcpy(sum, t, L * sizeof(uint32_t));
    for (k = 1;; k++) {
        pc_divs(t, tn, x2);
        while (tn > 0 && t[tn - 1] == 0) tn--;
        if (tn == 0) break;
        memcpy(w, t, tn * sizeof(uint32_t));
        pc_divs(w, tn, 2 * k + 1);
        if (k & 1) pc_sub(sum, w, tn, L);
        else pc_add(sum, w, tn, L);
    }
}

static size_t pc_bitlen(const uint32_t *x, size_t len) {
    size_t i = len;
    while (i > 0 && x[i - 1] == 0) i--;
    if (i == 0) return 0;
    {
        uint32_t v = x[i - 1];
        size_t b = 0;
        while (v) { b++; v >>= 1; }
        return 32 * (i - 1) + b;
    }
}

static int pc_cmp(const uint32_t *a, const uint32_t *b, size_t len) {
    size_t i = len;
    while (i-- > 0) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static uint64_t pc_top(const uint32_t *x, size_t len, size_t s) {
    size_t i = s / 32, off = s % 32, j;
    unsigned __int128 r = 0;
    for (j = 0; j < 3; j++)
        if (i + j < len) r |= ((unsigned __int128)x[i + j]) << (32 * j);
    return (uint64_t)(r >> off);
}

/* lower estimate of floor(a/b), at least 1, at most 2^32-1; requires a >= b > 0 */
static uint32_t pc_est(const uint32_t *a, const uint32_t *b, size_t len) {
    size_t na = pc_bitlen(a, len), nb = pc_bitlen(b, len);
    size_t sb = nb > 32 ? nb - 32 : 0;
    size_t sa = (na > 64 && na - 64 > sb) ? na - 64 : sb;
    uint64_t bt = pc_top(b, len, sb), at = pc_top(a, len, sa);
    uint64_t qe = at / (bt + 1);
    size_t sh = sa - sb;
    if (sh >= 32) qe = 0xFFFFFFFFull;
    else {
        if (qe > (0xFFFFFFFFull >> sh)) qe = 0xFFFFFFFFull;
        else qe <<= sh;
    }
    if (qe == 0) qe = 1;
    return (uint32_t)qe;
}

static int pc_iszero(const uint32_t *x, size_t len) {
    size_t i;
    for (i = 0; i < len; i++)
        if (x[i]) return 0;
    return 1;
}

static void pc_submul(uint32_t *a, const uint32_t *b, size_t len, uint32_t m) {
    uint64_t carry = 0, br = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t p = (uint64_t)b[i] * m + carry;
        uint64_t lo = (uint32_t)p, ai = a[i], s = lo + br;
        carry = p >> 32;
        a[i] = (uint32_t)(ai - s);
        br = ai < s ? 1 : 0;
    }
}

static size_t pc_euclid(uint32_t *a, uint32_t *b, size_t len, uint64_t *out, size_t maxc) {
    size_t cnt = 0;
    while (len > 0 && a[len - 1] == 0 && b[len - 1] == 0) len--;
    while (cnt < maxc) {
        uint64_t q = 0;
        uint32_t *tmp;
        if (pc_iszero(b, len)) break;
        while (pc_cmp(a, b, len) >= 0) {
            uint32_t m = pc_est(a, b, len);
            pc_submul(a, b, len, m);
            q += m;
        }
        out[cnt++] = q;
        tmp = a; a = b; b = tmp;
        while (len > 0 && a[len - 1] == 0) len--;
    }
    return cnt;
}

static void pc_muls(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static uint64_t *pc_q = NULL;
static size_t pc_qlen = 0;

static void pc_compute(size_t T) {
    size_t FL = (T * 4 + 256) / 32 + 2;
    for (;;) {
        size_t L = FL + 2, common = 0, c1, c2;
        uint32_t *A = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *w = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *d1 = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *d2 = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint64_t *q1 = (uint64_t *)malloc((T + 2) * sizeof(uint64_t));
        uint64_t *q2 = (uint64_t *)malloc((T + 2) * sizeof(uint64_t));
        uint32_t one[2];

        pc_atan_inv(A, L, FL, 5, t, w);
        pc_atan_inv(B, L, FL, 239, t, w);
        pc_muls(A, L, 16);
        pc_muls(B, L, 4);
        pc_sub(A, B, L, L);           /* A ~ Pi * 2^(32*FL) */
        memcpy(B, A, L * sizeof(uint32_t));
        one[0] = 0; one[1] = 1;       /* error margin 2^32 ulps */
        pc_sub(A, one, 2, L);         /* lower bound */
        pc_add(B, one, 2, L);         /* upper bound */
        d1[FL] = 1;
        d2[FL] = 1;

        c1 = pc_euclid(A, d1, L, q1, T + 2);
        c2 = pc_euclid(B, d2, L, q2, T + 2);
        while (common < c1 && common < c2 && q1[common] == q2[common]) common++;
        if (common > 0) common--;

        free(A); free(B); free(t); free(w); free(d1); free(d2); free(q2);
        if (common >= T) {
            free(pc_q);
            pc_q = q1;
            pc_qlen = common;
            return;
        }
        free(q1);
        FL *= 2;
    }
}

seq_t a(idx_t n) {
    seq_t p2, p1;
    idx_t i;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if ((size_t)(n - 1) > pc_qlen) {
        size_t T = (size_t)(n - 1);
        if (T < 2 * pc_qlen) T = 2 * pc_qlen;
        if (T < 1024) T = 1024;
        pc_compute(T);
    }
    p2 = 1;
    p1 = (seq_t)pc_q[0];
    for (i = 1; i + 1 < n; i++) {
        seq_t p = (seq_t)pc_q[i] * p1 + p2;
        p2 = p1;
        p1 = p;
    }
    return p1;
}
