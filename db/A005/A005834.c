/*
 * A005834  a(n) = floor(phi*a(n-2)) + a(n-1) where phi is the golden ratio.
 * https://oeis.org/A005834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(phi*a(n-2)) + a(n-1), a(0) = 0, a(1) = 2, phi =
 *            (1+sqrt(5))/2. floor(phi*x) is computed exactly with integers
 *            only: a Zeckendorf shift in fixed-width builds, and a
 *            fixed-point phi with an exact integer correction in the
 *            big-integer build
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 921 (2026-11-03)
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

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

/* Uses the Zeckendorf form x = sum F_k (with F_2 = 1, F_3 = 2, ...).
   Here S = sum F_{k+1}, and S - phi*x = sum psi^k, which lies in (-1,1).
   Its sign matches the smallest k, so floor(phi*x) is S - 1 when that k is
   even, and S when it is odd. */
static seq_t floor_phi_small(seq_t x) {
    seq_t f[200];
    int K, k, last = 0;
    seq_t r, S;
    if (x == 0) return (seq_t)0;
    f[1] = (seq_t)1;
    f[2] = (seq_t)1;
    K = 2;
    while (K < 198) {
        seq_t nx = f[K] + f[K - 1];
        f[K + 1] = nx;
        K++;
        if (nx > x || nx < f[K - 1]) break;
    }
    r = x;
    S = (seq_t)0;
    for (k = K - 1; k >= 2; k--) {
        if (f[k] <= r) {
            r -= f[k];
            S += f[k + 1];
            last = k;
            k--;
        }
    }
    return (last % 2 == 0) ? S - 1 : S;
}

static seq_t isqrt_seq(seq_t N, seq_t g) {
    /* g must be >= floor(sqrt(N)) */
    for (;;) {
        seq_t y = (g + N / g) / 2;
        if (y >= g) return g;
        g = y;
    }
}

static seq_t phip;
static idx_t pbits = 0;

static void ensure_phi(idx_t need) {
    seq_t pw, N, g, s;
    if (pbits >= need) return;
    pbits = need + need / 2 + 64;
    pw = (seq_t)1;
    pw <<= pbits;
    N = pw * pw * 5;
    g = pw * 4;
    s = isqrt_seq(N, g);
    phip = (pw + s) / 2; /* floor(phi * 2^pbits) */
}

static seq_t floor_phi_big(seq_t x) {
    seq_t f = (x * phip) >> pbits; /* <= floor(phi*x), off by at most a little */
    seq_t x5 = x * x * 5;
    for (;;) {
        /* f+1 > phi*x  <=>  2f+2-x > sqrt(5)*x */
        seq_t t = f * 2 + 2;
        if (t > x) {
            seq_t d = t - x;
            if (d * d > x5) break;
        }
        f += 1;
    }
    return f;
}

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;

    if (n >= memo_len) {
        idx_t old_len = memo_len;
        idx_t new_len = n + 1;
        idx_t i;
        bool big = seq_is_big() != 0;
        if (new_len < 2) new_len = 2;
        if (memo == NULL)
            memo = alloc_zero(new_len);
        else
            memo = grow_zero(memo, old_len, new_len);
        memo_len = new_len;
        if (big) ensure_phi(new_len + 16);

        for (i = old_len; i < new_len; i++) {
            if (i == 0) {
                memo[0] = (seq_t)0;
            } else if (i == 1) {
                memo[1] = (seq_t)2;
            } else {
                seq_t x = memo[i - 2];
                seq_t fp = big ? floor_phi_big(x) : floor_phi_small(x);
                memo[i] = fp + memo[i - 1];
            }
        }
    }

    return memo[n];
}
