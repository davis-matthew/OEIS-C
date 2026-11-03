/*
 * A002683  Numerators of coefficients for repeated integration.
 * https://oeis.org/A002683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of -(n/2)*M(n) - (2n+2)*M(n+1), where M(n) =
 *            (2/(2n+1)!)*Integral_{t=0..1} t*Product_{k=1..n} (t^2-k^2) =
 *            (1/(2n+1)!)*Integral_{u=0..1} Product_{k=1..n} (u-k^2) du.
 *            Evaluated exactly with internal multi-precision integers: X =
 *            -(n(2n+2)(2n+3)*S(n) + 4(n+1)*S(n+1)), Y = 2*L*(2n+3)...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* acc += a*b (acc zero-padded with enough capacity) */
static void q2683_addmul(uint32_t *acc, const uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    size_t i, j, k;
    for (i = 0; i < al; i++) {
        uint64_t carry = 0;
        if (a[i] == 0) continue;
        for (j = 0; j < bl; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + acc[i + j] + carry;
            acc[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        k = i + bl;
        while (carry) {
            uint64_t t = (uint64_t)acc[k] + carry;
            acc[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
}

static int q2683_cmp(const uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    size_t i;
    if (al != bl) return al < bl ? -1 : 1;
    for (i = al; i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

static uint32_t q2683_divmod(uint32_t *x, size_t len, uint32_t d, int write) {
    uint64_t r = 0;
    size_t i;
    for (i = len; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        if (write) x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    return (uint32_t)r;
}

static size_t q2683_trim(const uint32_t *x, size_t len) {
    while (len && x[len - 1] == 0) len--;
    return len;
}

/* a -= b, requires a >= b */
static size_t q2683_sub(uint32_t *a, size_t al, const uint32_t *b, size_t bl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < al; i++) {
        int64_t t = (int64_t)a[i] - borrow - (i < bl ? (int64_t)b[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
    return q2683_trim(a, al);
}

/* S = sum_j (-1)^(m-j) |c_j| * L/(j+1); result magnitude in pos, sign in *neg_out */
static size_t q2683_S(const uint32_t *poly, const size_t *plen, size_t W, idx_t m,
                      const uint32_t *L, size_t Ll, uint32_t *q,
                      uint32_t *pos, uint32_t *neg, size_t cap, int *neg_out) {
    idx_t j;
    size_t pl, nl, len;
    memset(pos, 0, cap * sizeof(uint32_t));
    memset(neg, 0, cap * sizeof(uint32_t));
    for (j = 0; j <= m; j++) {
        size_t ql;
        memcpy(q, L, Ll * sizeof(uint32_t));
        q2683_divmod(q, Ll, (uint32_t)(j + 1), 1);
        ql = q2683_trim(q, Ll);
        if ((m - j) & 1)
            q2683_addmul(neg, poly + j * W, plen[j], q, ql);
        else
            q2683_addmul(pos, poly + j * W, plen[j], q, ql);
    }
    pl = q2683_trim(pos, cap);
    nl = q2683_trim(neg, cap);
    if (q2683_cmp(pos, pl, neg, nl) >= 0) {
        len = q2683_sub(pos, pl, neg, nl);
        *neg_out = 0;
    } else {
        len = q2683_sub(neg, nl, pos, pl);
        memcpy(pos, neg, len * sizeof(uint32_t));
        *neg_out = 1;
    }
    return len;
}

/* x = x*m + y ; returns new length */
static size_t q2683_muladd(uint32_t *x, size_t xl, uint32_t m, const uint32_t *y, size_t yl) {
    size_t L = xl > yl ? xl : yl, i;
    uint64_t carry = 0;
    for (i = 0; i < L; i++) {
        uint64_t t = carry;
        if (i < xl) t += (uint64_t)x[i] * m;
        if (i < yl) t += y[i];
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) x[L++] = (uint32_t)carry;
    return q2683_trim(x, L);
}

seq_t a(idx_t n) {
    idx_t m1 = n + 1, N2 = n + 2, top = 2 * n + 3, k, j, p;
    unsigned char *comp = (unsigned char *)calloc((size_t)top + 1, 1);
    size_t Lcap, Ll, W, cap, sumbits = 0, l1, l2, Xl, i;
    uint32_t *L, *poly, *q, *pos, *neg, *S1, *S2;
    size_t *plen;
    int s1, s2, sx;
    seq_t v = 0;

    for (k = 2; k * k <= top; k++)
        if (!comp[k])
            for (j = k * k; j <= top; j += k) comp[j] = 1;

    /* L = lcm(1..n+2) */
    Lcap = (size_t)(N2 / 16) + 4;
    L = (uint32_t *)calloc(Lcap, sizeof(uint32_t));
    L[0] = 1; Ll = 1;
    for (p = 2; p <= N2; p++) {
        idx_t pp;
        if (comp[p]) continue;
        for (pp = p;; pp *= p) {
            Ll = q2683_muladd(L, Ll, (uint32_t)p, NULL, 0);
            if (pp > N2 / p) break;
        }
    }

    /* coefficient magnitude bound: prod (k^2+1) */
    for (k = 1; k <= m1; k++) {
        uint64_t t = (uint64_t)k * k + 1;
        while (t) { sumbits++; t >>= 1; }
    }
    W = sumbits / 32 + 2;
    poly = (uint32_t *)calloc((size_t)(m1 + 1) * W, sizeof(uint32_t));
    plen = (size_t *)calloc((size_t)(m1 + 1), sizeof(size_t));
    poly[0] = 1; plen[0] = 1;

    cap = W + Ll + 3;
    q = (uint32_t *)calloc(Ll + 1, sizeof(uint32_t));
    pos = (uint32_t *)calloc(cap, sizeof(uint32_t));
    neg = (uint32_t *)calloc(cap, sizeof(uint32_t));
    S1 = (uint32_t *)calloc(cap + 8, sizeof(uint32_t));
    S2 = (uint32_t *)calloc(cap + 8, sizeof(uint32_t));

    for (k = 1; k <= n; k++) {
        uint32_t k2 = (uint32_t)(k * k);
        for (j = k; j >= 1; j--)
            plen[j] = q2683_muladd(poly + j * W, plen[j], k2, poly + (j - 1) * W, plen[j - 1]);
        plen[0] = q2683_muladd(poly, plen[0], k2, NULL, 0);
    }
    l1 = q2683_S(poly, plen, W, n, L, Ll, q, pos, neg, cap, &s1);
    memcpy(S1, pos, l1 * sizeof(uint32_t));
    {
        uint32_t k2 = (uint32_t)(m1 * m1);
        for (j = m1; j >= 1; j--)
            plen[j] = q2683_muladd(poly + j * W, plen[j], k2, poly + (j - 1) * W, plen[j - 1]);
        plen[0] = q2683_muladd(poly, plen[0], k2, NULL, 0);
    }
    l2 = q2683_S(poly, plen, W, m1, L, Ll, q, pos, neg, cap, &s2);
    memcpy(S2, pos, l2 * sizeof(uint32_t));

    /* A = n(2n+2)(2n+3) S(n), B = 4(n+1) S(n+1) */
    l1 = q2683_muladd(S1, l1, (uint32_t)n, NULL, 0);
    l1 = q2683_muladd(S1, l1, (uint32_t)(2 * n + 2), NULL, 0);
    l1 = q2683_muladd(S1, l1, (uint32_t)(2 * n + 3), NULL, 0);
    l2 = q2683_muladd(S2, l2, 4u, NULL, 0);
    l2 = q2683_muladd(S2, l2, (uint32_t)(n + 1), NULL, 0);

    /* X = -(A + B), stored in S1 */
    if (s1 == s2) {
        Xl = q2683_muladd(S1, l1, 1u, S2, l2);
        sx = s1;
    } else if (q2683_cmp(S1, l1, S2, l2) >= 0) {
        Xl = q2683_sub(S1, l1, S2, l2);
        sx = s1;
    } else {
        Xl = q2683_sub(S2, l2, S1, l1);
        memcpy(S1, S2, Xl * sizeof(uint32_t));
        sx = s2;
    }
    sx = !sx;

    /* divide out common factors with Y = 2 * L * (2n+3)! */
    for (p = 2; p <= top && Xl > 0; p++) {
        idx_t e = 0, t, pp;
        if (comp[p]) continue;
        for (t = top; t; ) { t /= p; e += t; }
        if (p == 2) e++;
        if (p <= N2)
            for (pp = p;; pp *= p) {
                e++;
                if (pp > N2 / p) break;
            }
        while (e > 0 && q2683_divmod(S1, Xl, (uint32_t)p, 0) == 0) {
            q2683_divmod(S1, Xl, (uint32_t)p, 1);
            Xl = q2683_trim(S1, Xl);
            e--;
        }
    }

    for (i = Xl; i-- > 0;) {
        v = v * 65536 + (seq_t)(long long)(S1[i] >> 16);
        v = v * 65536 + (seq_t)(long long)(S1[i] & 0xFFFFu);
    }
    if (sx) v = -v;

    free(comp); free(L); free(poly); free(plen);
    free(q); free(pos); free(neg); free(S1); free(S2);
    return v;
}
