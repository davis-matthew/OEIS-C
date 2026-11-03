/*
 * A005324  Column of Motzkin triangle A026300.
 * https://oeis.org/A005324
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of paths s(0..n) with s(0)=0, s(n)=4, s(i)>=0,
 *            |s(i)-s(i-1)|<=1 (counted by dynamic programming over heights,
 *            additions only, for fixed-width seq_t); for arbitrary precision:
 *            a(n)*(n+6)*(n-4) = n*(2n+1)*a(n-1) + 3*n*(n-1)*a(n-2), a(4) = 1,
 *            a(5) = 5
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
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

static seq_t a_rec(idx_t n) {
    seq_t p2 = 1, p1 = 5;
    if (n == 4) return p2;
    if (n == 5) return p1;
    for (idx_t m = 6; m <= n; m++) {
        seq_t M = (seq_t)m;
        seq_t num = M * ((seq_t)2 * M + 1) * p1 + (seq_t)3 * M * (M - 1) * p2;
        seq_t cur = num / ((M + 6) * (M - 4));
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

static seq_t a_dp(idx_t n) {
    idx_t len = (n + 4) / 2 + 4;
    seq_t *v = alloc_zero(len);
    v[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        idx_t lim = i;
        if (n + 4 - i < lim) lim = n + 4 - i;
        if (lim > len - 2) lim = len - 2;
        seq_t prev = 0;
        for (idx_t h = 0; h <= lim; h++) {
            seq_t tmp = v[h];
            v[h] = prev + v[h] + v[h + 1];
            prev = tmp;
        }
    }
    seq_t r = v[4];
    release(v, len);
    return r;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (seq_is_fixed_width()) return a_dp(n);
    return a_rec(n);
}
