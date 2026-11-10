/*
 * A235955  Decimal expansion of log_Pi 10.
 * https://oeis.org/A235955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n-1) * log(10)/log(Pi)) mod 10, i.e. digits of
 *            1/A053511 = ln(10)/ln(Pi) (fixed-point bignum: Machin pi, atanh
 *            series for ln, Newton reciprocal)
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Validated: partial, pass, 111 terms through n = 111 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LB_BASE 1000000000u

static int g_L;

static uint32_t *lb_new(void) {
    return (uint32_t *)calloc((size_t)g_L + 1, sizeof(uint32_t));
}
static void lb_copy(uint32_t *d, const uint32_t *s) {
    memcpy(d, s, ((size_t)g_L + 1) * sizeof(uint32_t));
}
static int lb_zero(const uint32_t *a) {
    for (int i = 0; i <= g_L; i++) if (a[i]) return 0;
    return 1;
}
static void lb_add(uint32_t *a, const uint32_t *b) {
    uint32_t c = 0;
    for (int i = g_L; i >= 0; i--) {
        uint32_t s = a[i] + b[i] + c;
        if (s >= LB_BASE) { s -= LB_BASE; c = 1; } else c = 0;
        a[i] = s;
    }
}
static void lb_sub(uint32_t *a, const uint32_t *b) {
    int br = 0;
    for (int i = g_L; i >= 0; i--) {
        int64_t s = (int64_t)a[i] - b[i] - br;
        if (s < 0) { s += LB_BASE; br = 1; } else br = 0;
        a[i] = (uint32_t)s;
    }
}
static void lb_mul_small(uint32_t *a, uint32_t m) {
    uint64_t c = 0;
    for (int i = g_L; i >= 0; i--) {
        uint64_t cur = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)(cur % LB_BASE);
        c = cur / LB_BASE;
    }
}
static void lb_div_small(uint32_t *a, uint32_t m) {
    uint64_t rem = 0;
    for (int i = 0; i <= g_L; i++) {
        uint64_t cur = rem * LB_BASE + a[i];
        a[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
}
static void lb_mul(uint32_t *res, const uint32_t *a, const uint32_t *b) {
    memset(res, 0, ((size_t)g_L + 1) * sizeof(uint32_t));
    for (int i = 0; i <= g_L; i++) {
        if (!a[i]) continue;
        uint64_t carry = 0;
        for (int j = g_L - i; j >= 0; j--) {
            uint64_t cur = (uint64_t)res[i + j] + (uint64_t)a[i] * b[j] + carry;
            res[i + j] = (uint32_t)(cur % LB_BASE);
            carry = cur / LB_BASE;
        }
        int p = i - 1;
        while (carry && p >= 0) {
            uint64_t cur = (uint64_t)res[p] + carry;
            res[p] = (uint32_t)(cur % LB_BASE);
            carry = cur / LB_BASE;
            p--;
        }
    }
}
/* res = atanh(1/k) (alt=0) or atan(1/k) (alt=1) */
static void lb_series_inv(uint32_t *res, uint32_t k, int alt) {
    uint32_t *t = lb_new(), *tmp = lb_new();
    t[0] = 1;
    lb_div_small(t, k);
    lb_copy(res, t);
    uint32_t k2 = k * k;
    for (uint32_t j = 1;; j++) {
        lb_div_small(t, k2);
        if (lb_zero(t)) break;
        lb_copy(tmp, t);
        lb_div_small(tmp, 2 * j + 1);
        if (alt && (j & 1)) lb_sub(res, tmp); else lb_add(res, tmp);
    }
    free(t); free(tmp);
}

static uint8_t *g_dig = 0;
static idx_t g_cnt = 0; /* number of valid digits stored, index 0 = integer part digit */

static void compute(idx_t nd) {
    int L = (int)((nd + 8) / 9) + 4;
    g_L = L;
    uint32_t *A = lb_new(), *B = lb_new(), *pi = lb_new(), *ln2 = lb_new(), *ln10 = lb_new();
    uint32_t *lnpi = lb_new(), *y = lb_new(), *p = lb_new(), *tm = lb_new();
    uint32_t *pos = lb_new(), *neg = lb_new(), *r = lb_new(), *u = lb_new(), *q = lb_new();

    /* pi */
    lb_series_inv(A, 5, 1);  lb_mul_small(A, 16);
    lb_series_inv(B, 239, 1); lb_mul_small(B, 4);
    lb_copy(pi, A); lb_sub(pi, B);
    /* ln2 = 2 atanh(1/3) */
    lb_series_inv(ln2, 3, 0); lb_mul_small(ln2, 2);
    /* ln10 = 3 ln2 + 2 atanh(1/9) */
    lb_series_inv(A, 9, 0); lb_mul_small(A, 2);
    lb_copy(ln10, ln2); lb_mul_small(ln10, 3); lb_add(ln10, A);
    /* ln(25/8) = 2 ln10 - 5 ln2 */
    lb_copy(lnpi, ln10); lb_mul_small(lnpi, 2);
    lb_copy(A, ln2); lb_mul_small(A, 5);
    lb_sub(lnpi, A);
    /* y = pi*8/25 - 1 */
    lb_copy(y, pi); lb_mul_small(y, 8); lb_div_small(y, 25);
    y[0] -= 1;
    /* ln(1+y) */
    lb_copy(p, y); lb_copy(pos, y);
    for (uint32_t k = 2;; k++) {
        lb_mul(tm, p, y);
        lb_copy(p, tm);
        if (lb_zero(p)) break;
        lb_copy(tm, p);
        lb_div_small(tm, k);
        if (k & 1) lb_add(pos, tm); else lb_add(neg, tm);
    }
    lb_sub(pos, neg);
    lb_add(lnpi, pos);
    /* reciprocal of lnpi by Newton */
    memset(r, 0, ((size_t)g_L + 1) * sizeof(uint32_t));
    r[1] = 870000000u;
    {
        long long prec = 2;
        int it = 0;
        while (prec < (long long)L * 9 + 20) { prec *= 2; it++; }
        for (int i = 0; i < it + 2; i++) {
            lb_mul(tm, lnpi, r);
            memset(u, 0, ((size_t)g_L + 1) * sizeof(uint32_t));
            u[0] = 2;
            lb_sub(u, tm);
            lb_mul(tm, r, u);
            lb_copy(r, tm);
        }
    }
    lb_mul(q, ln10, r);

    idx_t usable = (idx_t)(L - 3) * 9;
    uint8_t *d = (uint8_t *)malloc((size_t)usable + 2);
    d[0] = (uint8_t)q[0];
    for (idx_t k = 1; k <= usable; k++) {
        idx_t limb = 1 + (k - 1) / 9;
        int pp = (int)((k - 1) % 9);
        uint32_t v = q[limb];
        for (int s = 0; s < 8 - pp; s++) v /= 10;
        d[k] = (uint8_t)(v % 10);
    }
    free(g_dig);
    g_dig = d;
    g_cnt = usable;

    free(A); free(B); free(pi); free(ln2); free(ln10); free(lnpi); free(y);
    free(p); free(tm); free(pos); free(neg); free(r); free(u); free(q);
}

seq_t a(idx_t n) {
    idx_t k = n - 1; /* digit index after the integer digit */
    if (!g_dig || k + 25 > g_cnt) {
        idx_t want = k + 40;
        if (want < 200) want = 200;
        if (g_cnt * 2 > want && g_cnt < k + 25) want = g_cnt * 2;
        compute(want);
    }
    return (seq_t)g_dig[k];
}
