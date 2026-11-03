/*
 * A005354  Number of asymmetric planar trees with n nodes.
 * https://oeis.org/A005354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = a(1) = 1; for n >= 2, a(n) = A022553(n-1) - (A000108(n-1)
 *            + [n even]*A000108(n/2-1))/2, where A022553(m) = (1/(2m)) *
 *            Sum_{d|m} mu(m/d)*binomial(2d,d) and A000108(k) =
 *            binomial(2k,k)/(k+1). Exact multi-limb integer arithmetic, then
 *            converted to seq_t
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 40, u128 n <= 73,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 964 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static void b_add(uint32_t *a, size_t *la, const uint32_t *b, size_t lb) {
    size_t n = (*la > lb) ? *la : lb;
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t x = (i < *la) ? a[i] : 0;
        uint64_t y = (i < lb) ? b[i] : 0;
        c += x + y;
        a[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) a[n++] = (uint32_t)c;
    *la = n;
}

static void b_trim(uint32_t *a, size_t *len) {
    while (*len > 1 && a[*len - 1] == 0) (*len)--;
}

static void b_div(uint32_t *a, size_t *len, uint32_t x) {
    uint64_t r = 0;
    size_t i = *len;
    while (i-- > 0) {
        r = (r << 32) | a[i];
        a[i] = (uint32_t)(r / x);
        r %= x;
    }
    b_trim(a, len);
}

static void b_mul(uint32_t *a, size_t *len, uint32_t x) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        c += (uint64_t)a[i] * x;
        a[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) a[(*len)++] = (uint32_t)c;
}

static void b_central(uint32_t *a, size_t *len, idx_t d) {
    idx_t i;
    a[0] = 1;
    *len = 1;
    for (i = 1; i <= d; i++) {
        b_mul(a, len, (uint32_t)(d + i));
        b_div(a, len, (uint32_t)i);
    }
}

/* a -= b, assuming a >= b */
static void b_sub(uint32_t *a, size_t *la, const uint32_t *b, size_t lb) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < *la; i++) {
        int64_t x = (int64_t)a[i] - (i < lb ? (int64_t)b[i] : 0) - br;
        if (x < 0) { x += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)x;
    }
    b_trim(a, la);
}

static seq_t b_to_seq(const uint32_t *a, size_t len) {
    seq_t r = 0;
    size_t k = len;
    while (k-- > 0) {
        r = (seq_t)(r << 8);
        r = (seq_t)(r << 8);
        r += (seq_t)(unsigned)(a[k] >> 16);
        r = (seq_t)(r << 8);
        r = (seq_t)(r << 8);
        r += (seq_t)(unsigned)(a[k] & 0xFFFFu);
    }
    return r;
}

static int mobius(idx_t k) {
    int mu = 1;
    idx_t p;
    for (p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            k /= p;
            if (k % p == 0) return 0;
            mu = -mu;
        }
    }
    if (k > 1) mu = -mu;
    return mu;
}

seq_t a(idx_t n) {
    idx_t m, d;
    size_t cap, lp, ln, lt;
    uint32_t *pos, *neg, *t;
    seq_t r;

    if (n < 2) return 1;
    m = n - 1;
    cap = (size_t)(n / 16 + 8);
    pos = (uint32_t *)calloc(cap, sizeof(uint32_t));
    neg = (uint32_t *)calloc(cap, sizeof(uint32_t));
    t = (uint32_t *)calloc(cap, sizeof(uint32_t));
    lp = 1; ln = 1; lt = 1;

    for (d = 1; d <= m; d++) {
        int mu;
        if (m % d != 0) continue;
        mu = mobius(m / d);
        if (mu == 0) continue;
        b_central(t, &lt, d);
        if (mu > 0) b_add(pos, &lp, t, lt);
        else b_add(neg, &ln, t, lt);
    }
    b_sub(pos, &lp, neg, ln);
    b_div(pos, &lp, (uint32_t)(2 * m));          /* A022553(m) */

    b_central(t, &lt, m);
    b_div(t, &lt, (uint32_t)(m + 1));            /* A000108(n-1) */
    if (n % 2 == 0) {
        b_central(neg, &ln, n / 2 - 1);
        b_div(neg, &ln, (uint32_t)(n / 2));      /* A000108(n/2-1) */
        b_add(t, &lt, neg, ln);
    }
    b_div(t, &lt, 2);
    b_sub(pos, &lp, t, lt);

    r = b_to_seq(pos, lp);
    free(pos);
    free(neg);
    free(t);
    return r;
}
