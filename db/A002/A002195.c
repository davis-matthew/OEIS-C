/*
 * A002195  Numerators of coefficients for numerical integration.
 * https://oeis.org/A002195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of (2/(2n+1)!)*Integral_{t=0..1}
 *            t*Product_{k=1..n} (t^2-k^2) = numerator of (1/(2n+1)!) *
 *            Sum_{j=0..n} [u^j]Product_{k=1..n}(u-k^2) / (j+1) (substituting
 *            u = t^2; exact big-integer arithmetic, reduced by primes <=
 *            2n+1)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   bit_length_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bit_length_u64.h"

static size_t p2195_norm(const uint32_t *x, size_t len)
{
    while (len > 0 && x[len - 1] == 0) len--;
    return len;
}

/* acc += x*y */
static void p2195_addmul(uint32_t *acc, size_t *al, size_t cap,
                         const uint32_t *x, size_t xl, const uint32_t *y, size_t yl)
{
    size_t i, j;
    for (i = 0; i < yl; i++) {
        uint64_t carry = 0, yi = y[i];
        size_t k;
        if (!yi) continue;
        for (j = 0; j < xl; j++) {
            uint64_t t = (uint64_t)acc[i + j] + (uint64_t)x[j] * yi + carry;
            acc[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        k = i + xl;
        while (carry && k < cap) {
            uint64_t t = (uint64_t)acc[k] + carry;
            acc[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
    *al = p2195_norm(acc, cap);
}

static int p2195_cmp(const uint32_t *a, size_t al, const uint32_t *b, size_t bl)
{
    size_t i;
    if (al != bl) return al < bl ? -1 : 1;
    i = al;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

/* q = x / d, returns remainder */
static uint32_t p2195_divsmall(const uint32_t *x, size_t xl, uint32_t d, uint32_t *q)
{
    uint64_t r = 0;
    size_t i = xl;
    while (i > 0) {
        i--;
        r = (r << 32) | x[i];
        if (q) q[i] = (uint32_t)(r / d);
        r %= d;
    }
    return (uint32_t)r;
}

/* x = x*s + y */
static void p2195_muladd(uint32_t *x, size_t *xl, uint32_t s, const uint32_t *y, size_t yl)
{
    size_t L = *xl > yl ? *xl : yl, i;
    uint64_t carry = 0;
    for (i = 0; i < L; i++) {
        uint64_t t = (i < *xl ? (uint64_t)x[i] * s : 0) + (i < yl ? (uint64_t)y[i] : 0) + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) x[L++] = (uint32_t)carry;
    *xl = p2195_norm(x, L);
}

/* a -= b, requires a >= b */
static void p2195_sub(uint32_t *a, size_t *al, const uint32_t *b, size_t bl)
{
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < *al; i++) {
        int64_t t = (int64_t)a[i] - (i < bl ? (int64_t)b[i] : 0) - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
    *al = p2195_norm(a, *al);
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n, k, j, CP, CL, CA;
    uint64_t bitsP = 1;
    uint32_t *c, *Lb, *q, *pos, *neg, *r, *tmp;
    size_t *clen, Ll, ql, posl = 0, negl = 0, rl;
    int negative = 0;
    char *comp;
    size_t M = 2 * N + 1, p;
    seq_t res = 0;

    for (k = 1; k <= N; k++) bitsP += (uint64_t)bit_length_u64((uint64_t)k * k + 1);
    CP = (size_t)(bitsP / 32) + 2;
    CL = (2 * (N + 1)) / 32 + 4;
    CA = CP + CL + 4;

    /* coefficients of Product_{k=1..n} (u + k^2) */
    c = (uint32_t *)calloc((N + 1) * CP, sizeof(uint32_t));
    clen = (size_t *)calloc(N + 1, sizeof(size_t));
    c[0] = 1; clen[0] = 1;
    for (k = 1; k <= N; k++) {
        uint32_t s = (uint32_t)(k * k);
        for (j = k; j >= 1; j--)
            p2195_muladd(c + j * CP, &clen[j], s, c + (j - 1) * CP, clen[j - 1]);
        p2195_muladd(c, &clen[0], s, NULL, 0);
    }

    /* primes up to 2n+1 */
    comp = (char *)calloc(M + 2, 1);
    for (p = 2; p * p <= M; p++)
        if (!comp[p])
            for (k = p * p; k <= M; k += p) comp[k] = 1;

    /* L = lcm(1..n+1) */
    Lb = (uint32_t *)calloc(CL, sizeof(uint32_t));
    Lb[0] = 1; Ll = 1;
    for (p = 2; p <= N + 1; p++) {
        size_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= N + 1; pk *= p) {
            p2195_muladd(Lb, &Ll, (uint32_t)p, NULL, 0);
            if (pk > (N + 1) / p) break;
        }
    }

    q = (uint32_t *)calloc(CL, sizeof(uint32_t));
    pos = (uint32_t *)calloc(CA, sizeof(uint32_t));
    neg = (uint32_t *)calloc(CA, sizeof(uint32_t));
    for (j = 0; j <= N; j++) {
        memset(q, 0, CL * sizeof(uint32_t));
        p2195_divsmall(Lb, Ll, (uint32_t)(j + 1), q);
        ql = p2195_norm(q, Ll);
        if (((N - j) & 1) == 0)
            p2195_addmul(pos, &posl, CA, c + j * CP, clen[j], q, ql);
        else
            p2195_addmul(neg, &negl, CA, c + j * CP, clen[j], q, ql);
    }

    if (p2195_cmp(pos, posl, neg, negl) >= 0) {
        p2195_sub(pos, &posl, neg, negl);
        r = pos; rl = posl;
    } else {
        p2195_sub(neg, &negl, pos, posl);
        r = neg; rl = negl;
        negative = 1;
    }

    /* divide out gcd with L*(2n+1)! */
    tmp = (uint32_t *)calloc(CA, sizeof(uint32_t));
    for (p = 2; p <= M && rl > 0; p++) {
        size_t e = 0, t;
        if (comp[p]) continue;
        for (t = M / p; t > 0; t /= p) e += t;
        for (t = p; t <= N + 1; t *= p) {
            e++;
            if (t > (N + 1) / p) break;
        }
        while (e > 0) {
            if (p2195_divsmall(r, rl, (uint32_t)p, NULL) != 0) break;
            memset(tmp, 0, CA * sizeof(uint32_t));
            p2195_divsmall(r, rl, (uint32_t)p, tmp);
            memcpy(r, tmp, rl * sizeof(uint32_t));
            rl = p2195_norm(r, rl);
            e--;
        }
    }

    for (k = rl; k > 0; k--) {
        uint32_t x = r[k - 1];
        res = res * 65536 + (idx_t)(x >> 16);
        res = res * 65536 + (idx_t)(x & 0xffffu);
    }
    if (negative) res = -res;

    free(c); free(clen); free(comp); free(Lb); free(q);
    free(pos); free(neg); free(tmp);
    return res;
}
