/*
 * A007560  Number of planted identity trees where non-root, non-leaf nodes an
 *          even distance from root are of degree 2.
 * https://oeis.org/A007560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(1) = 1, a(2) = 1, a(n) = b(n-2) for n >= 3, where Sum_{m>=0}
 *            b(m) x^m = Product_{k>=1} (1 + x^k)^a(k) (weigh transform),
 *            computed via b(0) = 1, b(m) = (1/m) * Sum_{i=1..m} c(i) b(m-i),
 *            c(i) = Sum_{d|i} (-1)^(i/d+1) d a(d). Fixed-width builds instead
 *            expand the product directly with exact binomi...
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 41, u64 n <= 75, u128 n <= 141,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 993 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

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

/* ---------- arbitrary precision path: weigh-transform recurrence ---------- */
static seq_t *ba = NULL, *bb = NULL, *bcp = NULL, *bcn = NULL;
static idx_t bcap = 0, bdone = 0;

static seq_t *grow_or_alloc(seq_t *old, idx_t oldlen, idx_t newlen) {
    if (old == NULL) return alloc_zero(newlen);
    return grow_zero(old, oldlen, newlen);
}

static void big_add(idx_t d, idx_t q, idx_t m) {
    seq_t t = (seq_t)d * ba[d];
    if (q & 1) bcp[m] += t;
    else bcn[m] += t;
}

static seq_t big_a(idx_t n) {
    if (n + 1 > bcap) {
        idx_t nc = bcap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        ba = grow_or_alloc(ba, bcap, nc);
        bb = grow_or_alloc(bb, bcap, nc);
        bcp = grow_or_alloc(bcp, bcap, nc);
        bcn = grow_or_alloc(bcn, bcap, nc);
        bcap = nc;
    }
    if (bdone == 0) {
        ba[1] = 1;
        ba[2] = 1;
        bb[0] = 1;
        bdone = 2;
    }
    for (idx_t i = bdone + 1; i <= n; i++) {
        idx_t m = i - 2;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                big_add(d, e, m);
                if (e != d) big_add(e, d, m);
            }
        }
        seq_t sp = 0, sn = 0;
        for (idx_t j = 1; j <= m; j++) {
            sp += bcp[j] * bb[m - j];
            sn += bcn[j] * bb[m - j];
        }
        bb[m] = (sp - sn) / (seq_t)m;
        ba[i] = bb[m];
        bdone = i;
    }
    return ba[n];
}

/* ---------- fixed width path: direct product expansion ---------- */
static seq_t *fa = NULL;
static idx_t fdone = 0, fcap = 0;

static seq_t fixed_a(idx_t n) {
    if (n > fdone) {
        idx_t N = n;
        if (N < 2 * fdone) N = 2 * fdone;
        if (N < 64) N = 64;
        if (fa != NULL) release(fa, fcap);
        fcap = N + 1;
        fa = alloc_zero(fcap);
        seq_t *P = alloc_zero(N);
        seq_t *C = alloc_zero(N);
        idx_t D = N - 2;
        fa[1] = 1;
        fa[2] = 1;
        P[0] = 1;
        for (idx_t i = 3; i <= N; i++) {
            idx_t k = i - 2;
            seq_t m = fa[k];
            idx_t J = D / k;
            C[0] = 1;
            idx_t jmax = 0;
            for (idx_t j = 1; j <= J; j++) {
                seq_t prev = C[j - 1];
                if (prev == 0) break;
                if (m < (seq_t)(j - 1) + 1) { C[j] = 0; break; }
                seq_t g = gcd(prev, (seq_t)j);
                C[j] = (prev / g) * ((m - (seq_t)(j - 1)) / ((seq_t)j / g));
                jmax = j;
                if (C[j] == 0) break;
            }
            for (idx_t d = D; d >= k; d--) {
                seq_t s = 0;
                for (idx_t j = 1; j <= jmax && j * k <= d; j++)
                    s += C[j] * P[d - j * k];
                P[d] += s;
            }
            fa[i] = P[i - 2];
        }
        release(P, N);
        release(C, N);
        fdone = N;
    }
    return fa[n];
}

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    if (seq_is_big()) return big_a(n);
    return fixed_a(n);
}
