/*
 * A007256  McKay-Thompson series of class 6C for Monster (and, apart from
 *          signs, of class 12A).
 * https://oeis.org/A007256
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^(n+1)] Product_{k>=1} 1/((1+q^k)^6 * (1+q^(3k))^6) for
 *            n != 0, a(0) = 0 (i.e. T6C =
 *            (eta(q)*eta(q^3)/(eta(q^2)*eta(q^6)))^6 + 6). Big build: Euler
 *            transform recurrence m*P(m) = Sum_{j=1..m} c(j)*P(m-j), P(0) =
 *            1, c(j) = Sum_{d|j} d*b(d), b(d) = -6 + 6[2|d] - 6[3|d] +
 *            6[6|d]. Fixed-width...
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 48, i64 n <= 179, i128 n <= 669,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 927 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static idx_t bcoef(idx_t d)
{
    idx_t b = -6;
    if (d % 2 == 0) b += 6;
    if (d % 3 == 0) b -= 6;
    if (d % 6 == 0) b += 6;
    return b;
}

static seq_t *P = NULL;
static idx_t plen = 0;

static void ensure_fixed(idx_t need)
{
    idx_t cap = need, k, i;
    int r;
    seq_t *Q;
    if (cap < plen + plen / 2) cap = plen + plen / 2;
    Q = alloc_zero(cap);
    Q[0] = 1;
    for (k = 1; k < cap; k++)
        for (r = 0; r < 6; r++)
            for (i = k; i < cap; i++)
                Q[i] -= Q[i - k];
    for (k = 3; k < cap; k += 3)
        for (r = 0; r < 6; r++)
            for (i = k; i < cap; i++)
                Q[i] -= Q[i - k];
    if (P) release(P, plen);
    P = Q;
    plen = cap;
}

static void ensure_big(idx_t need)
{
    idx_t cap = need, j, d, m;
    idx_t *c;
    if (cap < plen + plen / 2) cap = plen + plen / 2;
    c = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    for (j = 0; j < cap; j++) c[j] = 0;
    for (d = 1; d < cap; d++) {
        idx_t v = d * bcoef(d);
        for (j = d; j < cap; j += d) c[j] += v;
    }
    if (P == NULL) {
        P = alloc_zero(cap);
        P[0] = 1;
        plen = 1;
    } else {
        P = grow_zero(P, plen, cap);
    }
    for (m = plen; m < cap; m++) {
        seq_t s = 0;
        for (j = 1; j <= m; j++)
            s += P[m - j] * c[j];
        P[m] = s / m;
    }
    plen = cap;
    free(c);
}

seq_t a(idx_t n)
{
    idx_t need;
    if (n == 0) return 0;
    need = n + 2;
    if (need > plen) {
        if (seq_is_fixed_width()) ensure_fixed(need);
        else ensure_big(need);
    }
    return P[n + 1];
}
