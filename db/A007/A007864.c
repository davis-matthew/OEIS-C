/*
 * A007864  Number of matrix bundles of codimension n (Euler transform of
 *          A001156).
 * https://oeis.org/A007864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} 1/(1-x^k)^A001156(k+1), i.e. a(n) =
 *            (1/n)*Sum_{k=1..n} c(k)*a(n-k), a(0) = 1, where c(k) = Sum_{d|k}
 *            d*A001156(d+1) and A001156 = partitions into squares
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

/* ---------- fixed-width path: exact 256-bit accumulation ---------- */
static u128 *fa = NULL, *fb = NULL, *fc = NULL;
static idx_t fcap = 0, fhi = 0;

static void f_tables(idx_t N) {
    idx_t cap = (fcap == 0) ? (N < 64 ? 64 : N) : (fcap * 2 > N ? fcap * 2 : N);
    u128 *p = (u128 *)calloc(cap + 2, sizeof(u128));
    p[0] = 1;
    for (idx_t s = 1; s * s <= cap + 1; s++) {
        idx_t q = s * s;
        for (idx_t i = q; i <= cap + 1; i++) p[i] += p[i - q];
    }
    free(fb); free(fc);
    fb = (u128 *)calloc(cap + 1, sizeof(u128));
    fc = (u128 *)calloc(cap + 1, sizeof(u128));
    for (idx_t k = 1; k <= cap; k++) fb[k] = p[k + 1];
    for (idx_t d = 1; d <= cap; d++) {
        u128 t = (u128)d * fb[d];
        for (idx_t k = d; k <= cap; k += d) fc[k] += t;
    }
    free(p);
    if (fa == NULL) {
        fa = (u128 *)calloc(cap + 1, sizeof(u128));
        fa[0] = 1;
        fhi = 0;
    } else {
        fa = (u128 *)realloc(fa, (cap + 1) * sizeof(u128));
        for (idx_t i = fcap + 1; i <= cap; i++) fa[i] = 0;
    }
    fcap = cap;
}

static void add_at(uint64_t *acc, int pos, u128 v) {
    u128 carry = 0;
    for (int i = pos; i < 4; i++) {
        u128 t = (u128)acc[i] + (uint64_t)v + carry;
        acc[i] = (uint64_t)t;
        carry = t >> 64;
        v >>= 64;
        if (v == 0 && carry == 0) break;
    }
}

static void mul_add(uint64_t *acc, u128 x, u128 y) {
    uint64_t x0 = (uint64_t)x, x1 = (uint64_t)(x >> 64);
    uint64_t y0 = (uint64_t)y, y1 = (uint64_t)(y >> 64);
    add_at(acc, 0, (u128)x0 * y0);
    if (x0 && y1) add_at(acc, 1, (u128)x0 * y1);
    if (x1 && y0) add_at(acc, 1, (u128)x1 * y0);
    if (x1 && y1) add_at(acc, 2, (u128)x1 * y1);
}

static u128 f_a(idx_t n) {
    if (n > fcap) f_tables(n);
    for (idx_t i = fhi + 1; i <= n; i++) {
        uint64_t acc[4] = {0, 0, 0, 0};
        for (idx_t k = 1; k <= i; k++) mul_add(acc, fc[k], fa[i - k]);
        uint64_t q[4];
        u128 rem = 0;
        for (int l = 3; l >= 0; l--) {
            u128 cur = (rem << 64) | acc[l];
            q[l] = (uint64_t)(cur / i);
            rem = cur % i;
        }
        fa[i] = ((u128)q[1] << 64) | q[0];
    }
    if (n > fhi) fhi = n;
    return fa[n];
}

/* ---------- arbitrary-precision path ---------- */
static seq_t *sa = NULL, *sb = NULL, *sc = NULL;
static idx_t scap = 0, shi = 0;

static void s_tables(idx_t N) {
    idx_t cap = (scap == 0) ? (N < 64 ? 64 : N) : (scap * 2 > N ? scap * 2 : N);
    seq_t *p = alloc_zero(cap + 2);
    p[0] = 1;
    for (idx_t s = 1; s * s <= cap + 1; s++) {
        idx_t q = s * s;
        for (idx_t i = q; i <= cap + 1; i++) p[i] += p[i - q];
    }
    if (sb) release(sb, scap + 1);
    if (sc) release(sc, scap + 1);
    sb = alloc_zero(cap + 1);
    sc = alloc_zero(cap + 1);
    for (idx_t k = 1; k <= cap; k++) sb[k] = p[k + 1];
    for (idx_t d = 1; d <= cap; d++) {
        seq_t t = (seq_t)d * sb[d];
        for (idx_t k = d; k <= cap; k += d) sc[k] += t;
    }
    release(p, cap + 2);
    if (sa == NULL) {
        sa = alloc_zero(cap + 1);
        sa[0] = 1;
        shi = 0;
    } else {
        sa = grow_zero(sa, scap + 1, cap + 1);
    }
    scap = cap;
}

static seq_t s_a(idx_t n) {
    if (n > scap) s_tables(n);
    for (idx_t i = shi + 1; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; k++) sum += sc[k] * sa[i - k];
        sa[i] = sum / (seq_t)i;
    }
    if (n > shi) shi = n;
    return sa[n];
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return u128_to_seq(f_a(n));
    return s_a(n);
}
