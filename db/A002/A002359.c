/*
 * A002359  Denominators of continued fraction convergents to cube root of 6.
 * https://oeis.org/A002359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), with a(-1) = 0, a(0) = 1, where
 *            c(n) is the n-th partial quotient of 6^(1/3) = [1; 1, 4, 2, 7,
 *            ...]. The partial quotients are computed exactly with Lagrange's
 *            method: start from P(x) = x^3 - 6, then repeatedly take k =
 *            floor(real root of P) and replace P(x) with x^3...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 17, u64 n <= 31, u128 n <= 72,
 *            big any n
 * Helpers:   a2359_cmpmag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a2359_cmpmag.h"

typedef struct { uint32_t *d; size_t n, cap; int neg; } a2359_bn;

static void a2359_reserve(a2359_bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a2359_trim(a2359_bn *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void a2359_setll(a2359_bn *x, long long v) {
    unsigned long long m = v < 0 ? 0ULL - (unsigned long long)v : (unsigned long long)v;
    a2359_reserve(x, 2);
    x->n = 0;
    x->neg = v < 0;
    while (m) { x->d[x->n++] = (uint32_t)m; m >>= 32; }
    a2359_trim(x);
}

static void a2359_copy(a2359_bn *x, const a2359_bn *y) {
    a2359_reserve(x, y->n + 1);
    if (y->n) memcpy(x->d, y->d, y->n * sizeof(uint32_t));
    x->n = y->n;
    x->neg = y->neg;
}

static void a2359_mulk(a2359_bn *x, uint64_t k) {
    unsigned __int128 c = 0;
    size_t i;
    if (k == 0 || x->n == 0) { x->n = 0; x->neg = 0; return; }
    for (i = 0; i < x->n; i++) {
        c += (unsigned __int128)x->d[i] * k;
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) {
        a2359_reserve(x, x->n + 1);
        x->d[x->n++] = (uint32_t)c;
        c >>= 32;
    }
}

/* a += (tneg ? -1 : 1) * |t| */
static void a2359_addsigned(a2359_bn *a, const uint32_t *t, size_t tn, int tneg) {
    size_t i;
    if (tn == 0) return;
    if (a->n == 0 || a->neg == tneg) {
        size_t m = a->n > tn ? a->n : tn;
        uint64_t c = 0;
        a2359_reserve(a, m + 1);
        for (i = a->n; i < m; i++) a->d[i] = 0;
        for (i = 0; i < m; i++) {
            c += (uint64_t)a->d[i] + (i < tn ? t[i] : 0);
            a->d[i] = (uint32_t)c;
            c >>= 32;
        }
        a->n = m;
        if (c) a->d[a->n++] = (uint32_t)c;
        a->neg = tneg;
    } else {
        int cmp = a2359_cmpmag(a->d, a->n, t, tn);
        int64_t br = 0;
        if (cmp >= 0) {
            for (i = 0; i < a->n; i++) {
                int64_t v = (int64_t)a->d[i] - (int64_t)(i < tn ? t[i] : 0) - br;
                br = v < 0;
                if (v < 0) v += ((int64_t)1 << 32);
                a->d[i] = (uint32_t)v;
            }
        } else {
            size_t an = a->n;
            a2359_reserve(a, tn + 1);
            for (i = 0; i < tn; i++) {
                int64_t ai = i < an ? (int64_t)a->d[i] : 0;
                int64_t v = (int64_t)t[i] - ai - br;
                br = v < 0;
                if (v < 0) v += ((int64_t)1 << 32);
                a->d[i] = (uint32_t)v;
            }
            a->n = tn;
            a->neg = tneg;
        }
    }
    a2359_trim(a);
}

static a2359_bn a2359_c[4];
static a2359_bn a2359_tmp, a2359_ev;
static bool a2359_init = false;
static uint64_t *a2359_pq = NULL;
static size_t a2359_npq = 0, a2359_cappq = 0;

static void a2359_addmul(a2359_bn *a, const a2359_bn *b, uint64_t k) {
    a2359_copy(&a2359_tmp, b);
    a2359_mulk(&a2359_tmp, k);
    a2359_addsigned(a, a2359_tmp.d, a2359_tmp.n, a2359_tmp.neg);
}

/* true iff k lies below the unique real root of P */
static int a2359_below(uint64_t k) {
    int i;
    a2359_copy(&a2359_ev, &a2359_c[3]);
    for (i = 2; i >= 0; i--) {
        a2359_mulk(&a2359_ev, k);
        a2359_addsigned(&a2359_ev, a2359_c[i].d, a2359_c[i].n, a2359_c[i].neg);
    }
    return a2359_ev.n != 0 && a2359_ev.neg != a2359_c[3].neg;
}

static void a2359_next_pq(void) {
    uint64_t k;
    int i, j;
    a2359_bn t;
    if (!a2359_below(2)) {
        k = 1;
    } else {
        uint64_t lo = 2, hi = 4;
        while (a2359_below(hi)) { lo = hi; hi *= 2; }
        while (hi - lo > 1) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (a2359_below(mid)) lo = mid; else hi = mid;
        }
        k = lo;
    }
    /* P(x) <- P(x + k) */
    for (i = 0; i < 3; i++)
        for (j = 2; j >= i; j--)
            a2359_addmul(&a2359_c[j], &a2359_c[j + 1], k);
    /* P(x) <- x^3 P(1/x) */
    t = a2359_c[0]; a2359_c[0] = a2359_c[3]; a2359_c[3] = t;
    t = a2359_c[1]; a2359_c[1] = a2359_c[2]; a2359_c[2] = t;
    if (a2359_npq == a2359_cappq) {
        a2359_cappq = a2359_cappq * 2 + 64;
        a2359_pq = (uint64_t *)realloc(a2359_pq, a2359_cappq * sizeof(uint64_t));
    }
    a2359_pq[a2359_npq++] = k;
}

#define A2359_QCAP 16384
static seq_t a2359_q[A2359_QCAP];
static idx_t a2359_nq = 0;

seq_t a(idx_t n) {
    seq_t q0, q1, q;
    idx_t i;
    if (!a2359_init) {
        a2359_setll(&a2359_c[3], 1);
        a2359_setll(&a2359_c[2], 0);
        a2359_setll(&a2359_c[1], 0);
        a2359_setll(&a2359_c[0], -6);
        a2359_init = true;
        a2359_q[0] = 1;
        a2359_nq = 1;
    }
    while ((idx_t)a2359_npq <= n) a2359_next_pq();
    while (a2359_nq < A2359_QCAP && a2359_nq <= n) {
        i = a2359_nq;
        q0 = (i >= 2) ? a2359_q[i - 2] : (seq_t)0;
        a2359_q[i] = (seq_t)a2359_pq[i] * a2359_q[i - 1] + q0;
        a2359_nq++;
    }
    if (n < a2359_nq) return a2359_q[n];
    q0 = a2359_q[a2359_nq - 2];
    q1 = a2359_q[a2359_nq - 1];
    q = q1;
    for (i = a2359_nq; i <= n; i++) {
        q = (seq_t)a2359_pq[i] * q1 + q0;
        q0 = q1;
        q1 = q;
    }
    return q;
}
