/*
 * A005663  Numerators of convergents to log_2(3) = log(3)/log(2).
 * https://oeis.org/A005663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, easy, nonn
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), with a(-2) = 0, a(-1) = 1, where
 *            log_2(3) = [c(0); c(1), c(2), ...] = [1; 1, 1, 2, 2, 3, 1, 5,
 *            ...]. The partial quotients c(k) are computed exactly: ln2 =
 *            2*atanh(1/3) and ln3 = ln2 + 2*atanh(1/5) are evaluated in fixed
 *            point with a rigorous error bound, and only the...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 79,
 *            big any n
 * Helpers:   bn_addsmall, bn_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_addsmall.h"
#include "../utils/bn_divs.h"

static size_t bn_len(const uint32_t *x, size_t L) {
    while (L && !x[L - 1]) L--;
    return L;
}

static uint64_t bn_bitlen(const uint32_t *x, size_t L) {
    size_t l = bn_len(x, L);
    uint32_t top;
    uint64_t b;
    if (!l) return 0;
    top = x[l - 1];
    b = (uint64_t)(l - 1) * 32;
    while (top) { b++; top >>= 1; }
    return b;
}

static int bn_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    size_t i = L;
    while (i-- > 0) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void bn_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)a[i] + b[i];
        a[i] = (uint32_t)c;
        c >>= 32;
    }
}

static uint64_t bn_bits(const uint32_t *x, size_t L, uint64_t s) {
    size_t w = (size_t)(s / 32);
    unsigned r = (unsigned)(s % 32);
    unsigned __int128 v = 0;
    int k;
    for (k = 0; k < 3; k++) {
        if (w + k < L) v |= (unsigned __int128)x[w + k] << (32 * k);
    }
    return (uint64_t)(v >> r);
}

static void bn_submul32(uint32_t *a, const uint32_t *b, size_t L, uint32_t m, size_t off) {
    uint64_t carry = 0, borrow = 0, t;
    size_t lb = bn_len(b, L), i;
    if (!m) return;
    for (i = 0; i < lb && i + off < L; i++) {
        uint64_t p = (uint64_t)b[i] * m + carry;
        carry = p >> 32;
        t = (uint64_t)a[i + off] - (uint32_t)p - borrow;
        a[i + off] = (uint32_t)t;
        borrow = (t >> 63) & 1;
    }
    for (i = lb + off; i < L && (carry || borrow); i++) {
        t = (uint64_t)a[i] - carry - borrow;
        a[i] = (uint32_t)t;
        borrow = (t >> 63) & 1;
        carry = 0;
    }
}

/* a <- a mod b, *q <- a div b; returns 0 if the quotient is too large */
static int bn_divmod(uint32_t *a, const uint32_t *b, size_t L, uint64_t *q) {
    *q = 0;
    for (;;) {
        uint64_t la, lb, sa, sb, at, bt, qe;
        if (bn_cmp(a, b, L) < 0) break;
        la = bn_bitlen(a, L);
        lb = bn_bitlen(b, L);
        if (la - lb > 62) return 0;
        sb = lb > 32 ? lb - 32 : 0;
        bt = bn_bits(b, L, sb) + 1;
        sa = la > 64 ? la - 64 : 0;
        at = bn_bits(a, L, sa);
        qe = at / bt;
        if (sa >= sb) qe <<= (sa - sb);
        else qe >>= (sb - sa);
        if (qe == 0) qe = 1;
        bn_submul32(a, b, L, (uint32_t)qe, 0);
        bn_submul32(a, b, L, (uint32_t)(qe >> 32), 1);
        *q += qe;
    }
    return 1;
}

static uint64_t atanh_fx(uint32_t *sum, uint32_t *pw, uint32_t *t, size_t L,
                         uint64_t B, uint32_t k) {
    uint64_t c = 1, i;
    memset(pw, 0, L * sizeof(uint32_t));
    pw[B / 32] = (uint32_t)1 << (B % 32);
    bn_divs(pw, pw, L, k);
    memcpy(sum, pw, L * sizeof(uint32_t));
    for (i = 1;; i++) {
        bn_divs(pw, pw, L, k * k);
        if (!bn_len(pw, L)) break;
        bn_divs(t, pw, L, (uint32_t)(2 * i + 1));
        bn_add(sum, t, L);
        c++;
    }
    return c;
}

static uint64_t *cf_terms = NULL;
static size_t cf_count = 0;
static uint64_t cf_lastB = 0;

static void cf_compute(uint64_t B) {
    size_t L = (size_t)(B / 32) + 3, cnt = 0, cap = 64;
    uint32_t *buf = (uint32_t *)calloc(8 * L, sizeof(uint32_t));
    uint32_t *pw = buf, *t = buf + L, *A3 = buf + 2 * L, *A5 = buf + 3 * L;
    uint32_t *a1 = buf + 4 * L, *b1 = buf + 5 * L, *a2 = buf + 6 * L, *b2 = buf + 7 * L;
    uint64_t c3, c5, E;
    uint64_t *out = (uint64_t *)malloc(cap * sizeof(uint64_t));

    c3 = atanh_fx(A3, pw, t, L, B, 3);
    c5 = atanh_fx(A5, pw, t, L, B, 5);
    E = 2 * (c3 + c5) + 16;

    /* D = 2*A3 (ln2), N = 2*A3 + 2*A5 (ln3); both underestimate by < E */
    memset(b2, 0, L * sizeof(uint32_t));
    bn_add(b2, A3, L); bn_add(b2, A3, L);              /* D */
    memcpy(a1, b2, L * sizeof(uint32_t));
    bn_add(a1, A5, L); bn_add(a1, A5, L);              /* N */
    memcpy(a2, a1, L * sizeof(uint32_t));
    bn_addsmall(a2, L, E);                             /* N + E */
    memcpy(b1, b2, L * sizeof(uint32_t));
    bn_addsmall(b1, L, E);                             /* D + E */

    for (;;) {
        uint64_t q1, q2;
        uint32_t *tmp;
        if (!bn_divmod(a1, b1, L, &q1)) break;
        if (!bn_divmod(a2, b2, L, &q2)) break;
        if (q1 != q2) break;
        if (!bn_len(a1, L) || !bn_len(a2, L)) break;
        if (cnt == cap) {
            cap *= 2;
            out = (uint64_t *)realloc(out, cap * sizeof(uint64_t));
        }
        out[cnt++] = q1;
        tmp = a1; a1 = b1; b1 = tmp;
        tmp = a2; a2 = b2; b2 = tmp;
    }
    free(buf);
    if (cnt > cf_count) {
        free(cf_terms);
        cf_terms = out;
        cf_count = cnt;
    } else {
        free(out);
    }
}

seq_t a(idx_t n) {
    seq_t p2 = 0, p1 = 1;
    idx_t k;
    while ((uint64_t)cf_count <= (uint64_t)n) {
        uint64_t B = 4 * (uint64_t)n + 256;
        if (B < 2 * cf_lastB) B = 2 * cf_lastB;
        cf_lastB = B;
        cf_compute(B);
    }
    for (k = 0; k <= n; k++) {
        seq_t p = (seq_t)cf_terms[k] * p1 + p2;
        p2 = p1;
        p1 = p;
    }
    return p1;
}
