/*
 * A064236  Number of decimal digits in A001042.
 * https://oeis.org/A064236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits of A001042(n), where A001042(n)
 *            = A001042(n-1)^2 - A001042(n-2)^2, A001042(0) = 1, A001042(1) =
 *            2. Exact base-1e9 bignum while terms are small; after that,
 *            rigorous interval bounds (truncated floating mantissa of K
 *            limbs) on repeated squaring, with K doubled until...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define A064236_BASE 1000000000u

static size_t A064236_add(const uint32_t *x, size_t la, const uint32_t *y, size_t lb, uint32_t *out) {
    uint32_t carry = 0;
    size_t i;
    for (i = 0; i < la; i++) {
        uint32_t s = x[i] + (i < lb ? y[i] : 0u) + carry;
        if (s >= A064236_BASE) { s -= A064236_BASE; carry = 1; } else carry = 0;
        out[i] = s;
    }
    if (carry) out[i++] = carry;
    return i;
}

static size_t A064236_sub(const uint32_t *x, size_t la, const uint32_t *y, size_t lb, uint32_t *out) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < la; i++) {
        int64_t s = (int64_t)x[i] - (i < lb ? (int64_t)y[i] : 0) - borrow;
        if (s < 0) { s += A064236_BASE; borrow = 1; } else borrow = 0;
        out[i] = (uint32_t)s;
    }
    while (la > 1 && out[la - 1] == 0) la--;
    return la;
}

static size_t A064236_mul(const uint32_t *x, size_t la, const uint32_t *y, size_t lb, uint32_t *out) {
    size_t i, j, len;
    for (i = 0; i < la + lb; i++) out[i] = 0;
    for (i = 0; i < la; i++) {
        uint64_t carry = 0, xi = x[i];
        if (xi == 0) continue;
        for (j = 0; j < lb; j++) {
            uint64_t cur = (uint64_t)out[i + j] + xi * (uint64_t)y[j] + carry;
            out[i + j] = (uint32_t)(cur % A064236_BASE);
            carry = cur / A064236_BASE;
        }
        j = i + lb;
        while (carry) {
            uint64_t cur = (uint64_t)out[j] + carry;
            out[j] = (uint32_t)(cur % A064236_BASE);
            carry = cur / A064236_BASE;
            j++;
        }
    }
    len = la + lb;
    while (len > 1 && out[len - 1] == 0) len--;
    return len;
}

/* digits of m * 1e9^e */
static uint64_t A064236_ndig(const uint32_t *m, size_t len, uint64_t e) {
    uint64_t d = (uint64_t)(len - 1) * 9u + 9u * e;
    uint32_t top = m[len - 1];
    do { d++; top /= 10; } while (top);
    return d;
}

/* square m*1e9^e, keep top K limbs; up=1 -> upper bound (+1 ulp),
   up=0 -> lower bound (-1 ulp, which also absorbs the subtracted term) */
static void A064236_sq(uint32_t *m, size_t *len, uint64_t *e, size_t K, int up) {
    uint32_t *tmp = (uint32_t *)malloc(2 * (*len) * sizeof(uint32_t));
    size_t L = A064236_mul(m, *len, m, *len, tmp);
    uint64_t ne = 2 * (*e);
    size_t i;
    if (L > K) {
        size_t drop = L - K;
        memcpy(m, tmp + drop, K * sizeof(uint32_t));
        *len = K;
        ne += drop;
    } else {
        memcpy(m, tmp, L * sizeof(uint32_t));
        *len = L;
    }
    free(tmp);
    if (up) {
        int over = 1;
        for (i = 0; i < *len; i++) {
            if (++m[i] < A064236_BASE) { over = 0; break; }
            m[i] = 0;
        }
        if (over) {
            if (*len < K) m[(*len)++] = 1;
            else { ne += *len; m[0] = 1; *len = 1; }
        }
    } else {
        i = 0;
        while (m[i] == 0) { m[i] = A064236_BASE - 1; i++; }
        m[i]--;
        while (*len > 1 && m[*len - 1] == 0) (*len)--;
    }
    *e = ne;
}

static uint64_t A064236_try(idx_t n, size_t K, int *ok) {
    uint32_t *p, *q, *s, *t, *r, *lo, *hi;
    size_t lp = 1, lq = 1, ls, lt, lr, llo, lhi, k;
    uint64_t elo, ehi, dl, dh;
    idx_t i, j;
    p = (uint32_t *)malloc(sizeof(uint32_t));
    q = (uint32_t *)malloc(sizeof(uint32_t));
    p[0] = 1; q[0] = 2;
    for (i = 2; ; i++) {
        s = (uint32_t *)malloc((lq + 1) * sizeof(uint32_t));
        t = (uint32_t *)malloc(lq * sizeof(uint32_t));
        ls = A064236_add(q, lq, p, lp, s);
        lt = A064236_sub(q, lq, p, lp, t);
        r = (uint32_t *)malloc((ls + lt) * sizeof(uint32_t));
        lr = A064236_mul(s, ls, t, lt, r);
        free(s); free(t); free(p);
        p = q; lp = lq;
        q = r; lq = lr;
        if (i == n) {
            dl = A064236_ndig(q, lq, 0);
            free(p); free(q);
            *ok = 1;
            return dl;
        }
        if (lq >= K + 5) break;
    }
    lo = (uint32_t *)malloc((K + 1) * sizeof(uint32_t));
    hi = (uint32_t *)malloc((K + 1) * sizeof(uint32_t));
    for (k = 0; k < K; k++) { lo[k] = q[lq - K + k]; hi[k] = lo[k]; }
    llo = lhi = K;
    elo = ehi = (uint64_t)(lq - K);
    free(p); free(q);
    {
        int over = 1;
        for (k = 0; k < lhi; k++) {
            if (++hi[k] < A064236_BASE) { over = 0; break; }
            hi[k] = 0;
        }
        if (over) { ehi += lhi; hi[0] = 1; lhi = 1; }
    }
    for (j = i + 1; j <= n; j++) {
        A064236_sq(lo, &llo, &elo, K, 0);
        A064236_sq(hi, &lhi, &ehi, K, 1);
    }
    dl = A064236_ndig(lo, llo, elo);
    dh = A064236_ndig(hi, lhi, ehi);
    free(lo); free(hi);
    *ok = (dl == dh);
    return dh;
}

seq_t a(idx_t n) {
    seq_t res = 0;
    size_t K = 8;
    uint64_t d;
    int ok = 0;
    if (n <= 1) { res += 1; return res; }
    for (;;) {
        d = A064236_try(n, K, &ok);
        if (ok || K >= 4096) break;
        K *= 2;
    }
    res += (idx_t)d;
    return res;
}
