/*
 * A003077  Decimal expansion of 22/7 - Pi.
 * https://oeis.org/A003077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = floor(10^(n+1) * (22/7 - Pi)) mod 10, with Pi =
 *            16*arctan(1/5) - 4*arctan(1/239) (Machin), evaluated in exact
 *            base-10^9 fixed-point arithmetic
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A3077_BASE 1000000000ULL

static unsigned char *a3077_dig = NULL;
static idx_t a3077_cnt = 0;

static void a3077_div(uint32_t *x, size_t L, size_t s, uint32_t d)
{
    uint64_t r = 0;
    size_t i;
    for (i = s; i < L; i++) {
        uint64_t c = r * A3077_BASE + x[i];
        x[i] = (uint32_t)(c / d);
        r = c % d;
    }
}

static void a3077_add(uint32_t *a, const uint32_t *b, size_t L)
{
    uint32_t carry = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t s = (uint64_t)a[i] + b[i] + carry;
        if (s >= A3077_BASE) { a[i] = (uint32_t)(s - A3077_BASE); carry = 1; }
        else { a[i] = (uint32_t)s; carry = 0; }
    }
}

static void a3077_sub(uint32_t *a, const uint32_t *b, size_t L)
{
    int64_t borrow = 0;
    size_t i = L;
    while (i-- > 0) {
        int64_t s = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (s < 0) { s += (int64_t)A3077_BASE; borrow = 1; }
        else borrow = 0;
        a[i] = (uint32_t)s;
    }
}

/* adds M*arctan(1/x): even-index terms to pos, odd-index terms to neg */
static void a3077_atan(uint32_t *pos, uint32_t *neg, size_t L,
                       uint32_t M, uint32_t x, uint32_t *t, uint32_t *q)
{
    size_t start = 0;
    uint32_t x2 = x * x;
    unsigned long k;
    memset(t, 0, L * sizeof(uint32_t));
    t[0] = M;
    a3077_div(t, L, 0, x);
    a3077_add(pos, t, L);
    for (k = 1;; k++) {
        a3077_div(t, L, start, x2);
        while (start < L && t[start] == 0) start++;
        if (start >= L) break;
        memcpy(q, t, L * sizeof(uint32_t));
        a3077_div(q, L, start, (uint32_t)(2 * k + 1));
        if (k & 1) a3077_add(neg, q, L);
        else a3077_add(pos, q, L);
    }
}

static void a3077_compute(idx_t digits)
{
    size_t L = (size_t)(digits / 9) + 6;
    uint32_t *pos = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t i, cnt;
    size_t j;

    /* pi = 16 atan(1/5) - 4 atan(1/239) */
    a3077_atan(pos, neg, L, 16, 5, t, q);
    a3077_atan(neg, pos, L, 4, 239, t, q);

    /* r = 22/7 + neg - pos */
    r[0] = 22;
    a3077_div(r, L, 0, 7);
    a3077_add(r, neg, L);
    a3077_sub(r, pos, L);

    cnt = (idx_t)(L - 4) * 9;
    free(a3077_dig);
    a3077_dig = (unsigned char *)malloc((size_t)cnt);
    i = 0;
    for (j = 1; j < L - 3; j++) {
        uint32_t v = r[j];
        int p;
        unsigned char buf[9];
        for (p = 8; p >= 0; p--) { buf[p] = (unsigned char)(v % 10); v /= 10; }
        for (p = 0; p < 9; p++) a3077_dig[i++] = buf[p];
    }
    a3077_cnt = i;

    free(pos); free(neg); free(t); free(q); free(r);
}

seq_t a(idx_t n)
{
    if (n >= a3077_cnt) {
        idx_t want = n + 64;
        if (want < 2 * a3077_cnt) want = 2 * a3077_cnt;
        if (want < 256) want = 256;
        a3077_compute(want);
    }
    return (seq_t)a3077_dig[n];
}
