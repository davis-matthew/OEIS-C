/*
 * A001467  Denominators of an expansion for Pi.
 * https://oeis.org/A001467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=a(1)=a(2)=1; r = Pi - 3; for n >= 3: a(n) = round(1/r),
 *            then r <- r - 1/a(n). Pi is computed exactly in binary fixed
 *            point (Machin: Pi = 16 atan(1/5) - 4 atan(1/239)) with about
 *            4*2^n bits, which is far more than the precision needed for a(n)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 6, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t lw;

static size_t nl(const lw *x, size_t L) { while (L && !x[L - 1]) L--; return L; }

static lw divs(lw *x, size_t len, lw d) {
    uint64_t r = 0; size_t i = len;
    while (i--) { uint64_t c = (r << 32) | x[i]; x[i] = (lw)(c / d); r = c % d; }
    return (lw)r;
}

static void muls(lw *x, size_t L, lw m) {
    uint64_t c = 0; size_t i;
    for (i = 0; i < L; i++) { uint64_t t = (uint64_t)x[i] * m + c; x[i] = (lw)t; c = t >> 32; }
}

static void addn(lw *a, const lw *b, size_t n, size_t L) {
    uint64_t c = 0; size_t i;
    for (i = 0; i < n; i++) { uint64_t t = (uint64_t)a[i] + b[i] + c; a[i] = (lw)t; c = t >> 32; }
    for (; c && i < L; i++) { uint64_t t = (uint64_t)a[i] + c; a[i] = (lw)t; c = t >> 32; }
}

static void subn(lw *a, const lw *b, size_t n, size_t L) {
    uint64_t br = 0; size_t i;
    for (i = 0; i < n; i++) { uint64_t t = (uint64_t)a[i] - b[i] - br; a[i] = (lw)t; br = (t >> 63) & 1; }
    for (; br && i < L; i++) { uint64_t t = (uint64_t)a[i] - br; a[i] = (lw)t; br = (t >> 63) & 1; }
}

static int cmpn(const lw *a, const lw *b, size_t L) {
    size_t i = L;
    while (i--) { if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1; }
    return 0;
}

/* q = floor(u / v), u has m limbs, v has n limbs (v[n-1] != 0); q must be zeroed */
static void kdiv(const lw *u, size_t m, const lw *v, size_t n, lw *q) {
    size_t i; long long j;
    if (m < n) return;
    if (n == 1) {
        uint64_t k = 0; i = m;
        while (i--) { uint64_t c = (k << 32) | u[i]; q[i] = (lw)(c / v[0]); k = c % v[0]; }
        return;
    }
    int s = 0; lw t0 = v[n - 1];
    while (!(t0 & 0x80000000u)) { t0 <<= 1; s++; }
    lw *vn = (lw *)malloc(n * sizeof(lw));
    lw *un = (lw *)malloc((m + 1) * sizeof(lw));
    for (i = n - 1; i > 0; i--) vn[i] = (v[i] << s) | (s ? v[i - 1] >> (32 - s) : 0);
    vn[0] = v[0] << s;
    un[m] = s ? u[m - 1] >> (32 - s) : 0;
    for (i = m - 1; i > 0; i--) un[i] = (u[i] << s) | (s ? u[i - 1] >> (32 - s) : 0);
    un[0] = u[0] << s;
    const uint64_t b = (uint64_t)1 << 32;
    for (j = (long long)(m - n); j >= 0; j--) {
        uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
        uint64_t qhat = num / vn[n - 1], rhat = num % vn[n - 1];
        while (qhat >= b || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
            qhat--; rhat += vn[n - 1];
            if (rhat >= b) break;
        }
        long long k = 0, t;
        for (i = 0; i < n; i++) {
            uint64_t p = qhat * vn[i];
            t = (long long)un[i + j] - k - (long long)(p & 0xFFFFFFFFu);
            un[i + j] = (lw)t;
            k = (long long)(p >> 32) - (t >> 32);
        }
        t = (long long)un[j + n] - k;
        un[j + n] = (lw)t;
        q[j] = (lw)qhat;
        if (t < 0) {
            uint64_t c = 0;
            q[j]--;
            for (i = 0; i < n; i++) {
                uint64_t s2 = (uint64_t)un[i + j] + vn[i] + c;
                un[i + j] = (lw)s2; c = s2 >> 32;
            }
            un[j + n] = (lw)(un[j + n] + c);
        }
    }
    free(vn); free(un);
}

static void atan_inv(lw *res, lw x, size_t L, uint64_t B, lw *term, lw *tmp) {
    uint64_t k;
    size_t top = L;
    memset(term, 0, L * sizeof(lw));
    term[B / 32] = (lw)1 << (B % 32);
    divs(term, L, x);
    memcpy(res, term, L * sizeof(lw));
    lw x2 = (lw)((uint64_t)x * x);
    for (k = 1;; k++) {
        top = nl(term, top); if (!top) break;
        divs(term, top, x2);
        top = nl(term, top); if (!top) break;
        memcpy(tmp, term, top * sizeof(lw));
        divs(tmp, top, (lw)(2 * k + 1));
        if (k & 1) subn(res, tmp, top, L); else addn(res, tmp, top, L);
    }
}

static lw **T_mag = NULL;
static size_t *T_len = NULL;
static int *T_sgn = NULL;
static idx_t T_max = 0;

static void set_pow2(lw *x, size_t L, uint64_t e) {
    memset(x, 0, L * sizeof(lw));
    x[e / 32] = (lw)1 << (e % 32);
}

static void compute(idx_t N) {
    uint64_t B = ((uint64_t)1 << (N + 2)) + 256;
    size_t L = (size_t)(B / 32 + 4), sz = (L + 2) * sizeof(lw);
    lw *P = (lw *)calloc(L + 2, sizeof(lw)), *A = (lw *)calloc(L + 2, sizeof(lw));
    lw *term = (lw *)calloc(L + 2, sizeof(lw)), *tmp = (lw *)calloc(L + 2, sizeof(lw));
    lw *U = (lw *)calloc(L + 2, sizeof(lw)), *V = (lw *)calloc(L + 2, sizeof(lw));
    lw *Q = (lw *)calloc(L + 2, sizeof(lw));
    idx_t k;
    int s = 1;

    atan_inv(P, 5, L, B, term, tmp); muls(P, L, 16);
    atan_inv(A, 239, L, B, term, tmp); muls(A, L, 4);
    subn(P, A, L, L);
    set_pow2(A, L, B); muls(A, L, 3);
    subn(P, A, L, L);

    if (T_mag) {
        for (k = 0; k <= T_max; k++) free(T_mag[k]);
        free(T_mag); free(T_len); free(T_sgn);
    }
    T_mag = (lw **)calloc(N + 1, sizeof(lw *));
    T_len = (size_t *)calloc(N + 1, sizeof(size_t));
    T_sgn = (int *)calloc(N + 1, sizeof(int));

    for (k = 3; k <= N; k++) {
        size_t ql;
        set_pow2(U, L, B + 1); addn(U, P, L, L);
        memcpy(V, P, L * sizeof(lw)); muls(V, L, 2);
        memset(Q, 0, sz);
        kdiv(U, nl(U, L), V, nl(V, L), Q);
        ql = nl(Q, L);
        T_mag[k] = (lw *)malloc((ql + 1) * sizeof(lw));
        memcpy(T_mag[k], Q, ql * sizeof(lw));
        T_len[k] = ql; T_sgn[k] = s;

        set_pow2(U, L, B);
        memset(A, 0, sz);
        kdiv(U, nl(U, L), Q, ql, A);
        if (cmpn(P, A, L) >= 0) subn(P, A, L, L);
        else { subn(A, P, L, L); memcpy(P, A, L * sizeof(lw)); s = -s; }
    }
    T_max = N;
    free(P); free(A); free(term); free(tmp); free(U); free(V); free(Q);
}

static seq_t to_seq(const lw *m, size_t len, int s) {
    seq_t x = 0;
    size_t i;
    if (len <= 1 || (len == 2 && m[1] < 0x80000000u)) {
        uint64_t u = len ? m[0] : 0;
        if (len == 2) u |= (uint64_t)m[1] << 32;
        long long v = (long long)u;
        return (seq_t)(s < 0 ? -v : v);
    }
    i = len;
    while (i--) {
        x = x * 65536 + (seq_t)(long long)(m[i] >> 16);
        x = x * 65536 + (seq_t)(long long)(m[i] & 0xFFFFu);
    }
    return s < 0 ? -x : x;
}

seq_t a(idx_t n) {
    if (n < 3) return 1;
    if (n > 40) return 0;
    if (T_mag == NULL || n > T_max) compute(n);
    return to_seq(T_mag[n], T_len[n], T_sgn[n]);
}
