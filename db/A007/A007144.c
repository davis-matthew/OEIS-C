/*
 * A007144  E-trees with exactly 3 colors.
 * https://oeis.org/A007144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007142(n) - 3*A007141(n) + 3*A003238(n) for n > 1, a(1)
 *            = 0, where the k-colored E-tree counts F_k satisfy F_k(x) = x *
 *            B_k(x)^k with B_k(x) = 1 + Sum_{m>=1} F_k(x^m), i.e. F_k(1) = 1,
 *            F_k(n+1) = [x^n] B_k(x)^k, [x^n] B_k = Sum_{d|n} F_k(d) (k = 1
 *            gives A003238, k = 2 gives A007141, k = 3...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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

static seq_t divsum(const seq_t *f, idx_t n)
{
    seq_t s = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            s += f[d];
            if (d != n / d) s += f[n / d];
        }
    }
    return s;
}

static seq_t *F1 = NULL, *F2 = NULL, *F3 = NULL;   /* F_k[1..have] */
static seq_t *B1 = NULL, *B2 = NULL, *B3 = NULL;   /* B_k[0..have-1] */
static seq_t *P2 = NULL, *P3 = NULL, *R3 = NULL;   /* B2^2, B3^2, B3^3 */
static idx_t have = 0, cap = 0;

static seq_t *grow_one(seq_t *v, idx_t oldlen, idx_t newlen)
{
    if (v == NULL) return alloc_zero(newlen);
    return grow_zero(v, oldlen, newlen);
}

static void ensure(idx_t N)
{
    if (N + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < N + 1) nc = N + 1;
        if (nc < 16) nc = 16;
        F1 = grow_one(F1, cap, nc); F2 = grow_one(F2, cap, nc); F3 = grow_one(F3, cap, nc);
        B1 = grow_one(B1, cap, nc); B2 = grow_one(B2, cap, nc); B3 = grow_one(B3, cap, nc);
        P2 = grow_one(P2, cap, nc); P3 = grow_one(P3, cap, nc); R3 = grow_one(R3, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        F1[1] = 1; F2[1] = 1; F3[1] = 1;
        B1[0] = 1; B2[0] = 1; B3[0] = 1;
        P2[0] = 1; P3[0] = 1; R3[0] = 1;
        have = 1;
    }
    while (have < N) {
        idx_t n = have, i;
        seq_t s;

        B1[n] = divsum(F1, n);
        F1[n + 1] = B1[n];

        B2[n] = divsum(F2, n);
        s = 0;
        for (i = 0; i <= n; i++) s += B2[i] * B2[n - i];
        P2[n] = s;
        F2[n + 1] = s;

        B3[n] = divsum(F3, n);
        s = 0;
        for (i = 0; i <= n; i++) s += B3[i] * B3[n - i];
        P3[n] = s;
        s = 0;
        for (i = 0; i <= n; i++) s += B3[i] * P3[n - i];
        R3[n] = s;
        F3[n + 1] = s;

        have++;
    }
}

seq_t a(idx_t n)
{
    seq_t r;
    if (n <= 1) return 0;
    ensure(n);
    r = F3[n] + 3 * F1[n];
    r -= 3 * F2[n];
    return r;
}
