/*
 * A003454  Number of nonequivalent dissections of an n-gon by nonintersecting
 *          diagonals rooted at a cell up to rotation.
 * https://oeis.org/A003454
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*Sum_{k|n} phi(k)*D(n/k-1) - s(n) - (s(n) + s(n-1) +
 *            2*[n even]*s(n/2))/4, where D = A001850 (central Delannoy:
 *            m*D(m) = 3*(2m-1)*D(m-1) - (m-1)*D(m-2), D(0)=1, D(1)=3) and
 *            s(n) = A001003(n-1) (little Schroeder: (k+1)*S(k) =
 *            3*(2k-1)*S(k-1) - (k-2)*S(k-2), S(0)=S(1)=1). Derived from t...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 184 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a3454_add(uint32_t *a, size_t *al, const uint32_t *b, size_t bl)
{
    size_t L = *al > bl ? *al : bl, i;
    uint64_t c = 0;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)(i < *al ? a[i] : 0) + (i < bl ? b[i] : 0) + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) a[L++] = (uint32_t)c;
    *al = L;
}

static size_t a3454_nrm(const uint32_t *x, size_t len)
{
    while (len && x[len - 1] == 0) len--;
    return len;
}

static void a3454_mulsm(uint32_t *x, size_t *len, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x[(*len)++] = (uint32_t)c;
    *len = a3454_nrm(x, *len);
}

static void a3454_accum(uint32_t *acc, size_t *al, const uint32_t *D, size_t dl,
                        uint32_t ph, uint32_t *tmp)
{
    size_t lt = dl;
    memcpy(tmp, D, dl * sizeof(uint32_t));
    a3454_mulsm(tmp, &lt, ph);
    a3454_add(acc, al, tmp, lt);
}

static void a3454_div(uint32_t *x, size_t *len, uint32_t d)
{
    uint64_t r = 0;
    size_t i = *len;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    *len = a3454_nrm(x, *len);
}

static uint32_t a3454_phi(uint64_t m)
{
    uint64_t r = m, p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return (uint32_t)r;
}

/* a -= b, requires a >= b */
static void a3454_sub(uint32_t *a, size_t *al, const uint32_t *b, size_t bl)
{
    size_t i;
    int64_t br = 0;
    for (i = 0; i < *al; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)(i < bl ? b[i] : 0) - br;
        if (t < 0) { t += (int64_t)1 << 32; br = 1; } else br = 0;
        a[i] = (uint32_t)t;
    }
    *al = a3454_nrm(a, *al);
}

/* out = (P1*c1 - P2*c2) / dv */
static void a3454_step(const uint32_t *P2, size_t l2, const uint32_t *P1, size_t l1,
                       uint32_t c1, uint32_t c2, uint32_t dv,
                       uint32_t *out, size_t *lo, uint32_t *tmp)
{
    size_t lt = l2;
    memcpy(out, P1, l1 * sizeof(uint32_t));
    *lo = l1;
    a3454_mulsm(out, lo, c1);
    memcpy(tmp, P2, l2 * sizeof(uint32_t));
    a3454_mulsm(tmp, &lt, c2);
    a3454_sub(out, lo, tmp, lt);
    a3454_div(out, lo, dv);
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    uint64_t N = (uint64_t)n;
    size_t cap = (size_t)(3 * N / 32 + 8);
    uint32_t *acc = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *A = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *C = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *sn = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *sn1 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *se = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t accl = 0, la, lb, lc, lsn = 0, lsn1 = 0, lse = 0;
    uint64_t m, k;
    uint32_t *sw;

    /* Delannoy part */
    A[0] = 1; la = 1;
    a3454_accum(acc, &accl, A, la, a3454_phi(N), T);
    B[0] = 3; lb = 1;
    if (N % 2 == 0) a3454_accum(acc, &accl, B, lb, a3454_phi(N / 2), T);
    for (m = 2; m + 1 <= N; m++) {
        a3454_step(A, la, B, lb, (uint32_t)(3 * (2 * m - 1)), (uint32_t)(m - 1),
                   (uint32_t)m, C, &lc, T);
        sw = A; A = B; la = lb; B = C; lb = lc; C = sw;
        if (N % (m + 1) == 0)
            a3454_accum(acc, &accl, B, lb, a3454_phi(N / (m + 1)), T);
    }
    a3454_div(acc, &accl, (uint32_t)N);

    /* little Schroeder part: need S(n-1), S(n-2), S(n/2-1) */
    memset(A, 0, cap * sizeof(uint32_t));
    memset(B, 0, cap * sizeof(uint32_t));
    A[0] = 1; la = 1;
    B[0] = 1; lb = 1;
    for (k = 0; k + 1 <= N; k++) {
        const uint32_t *cur;
        size_t lcur;
        if (k >= 2) {
            a3454_step(A, la, B, lb, (uint32_t)(3 * (2 * k - 1)), (uint32_t)(k - 2),
                       (uint32_t)(k + 1), C, &lc, T);
            sw = A; A = B; la = lb; B = C; lb = lc; C = sw;
            cur = B; lcur = lb;
        } else if (k == 1) {
            cur = B; lcur = lb;
        } else {
            cur = A; lcur = la;
        }
        if (k == N - 1) { memcpy(sn, cur, lcur * sizeof(uint32_t)); lsn = lcur; }
        if (k == N - 2) { memcpy(sn1, cur, lcur * sizeof(uint32_t)); lsn1 = lcur; }
        if (N % 2 == 0 && k == N / 2 - 1) { memcpy(se, cur, lcur * sizeof(uint32_t)); lse = lcur; }
    }

    /* T = (sn + sn1 + 2*se) / 4 */
    {
        size_t lt = lsn;
        memcpy(T, sn, lsn * sizeof(uint32_t));
        a3454_add(T, &lt, sn1, lsn1);
        if (N % 2 == 0) {
            a3454_mulsm(se, &lse, 2);
            a3454_add(T, &lt, se, lse);
        }
        a3454_div(T, &lt, 4);
        a3454_sub(acc, &accl, sn, lsn);
        a3454_sub(acc, &accl, T, lt);
    }

    seq_t v = 0;
    {
        size_t i = accl;
        while (i > 0) {
            int b;
            i--;
            for (b = 3; b >= 0; b--) {
                v = (seq_t)(v << 8);
                v += (seq_t)((acc[i] >> (8 * b)) & 255u);
            }
        }
    }

    free(acc); free(A); free(B); free(C); free(T);
    free(sn); free(sn1); free(se);
    return v;
}
