/*
 * A005909  a(n) = floor(tau*a(n-1)) + floor(tau*a(n-2)) with a(0)=0 and
 *          a(1)=2.
 * https://oeis.org/A005909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(phi*a(n-1)) + floor(phi*a(n-2)), a(0) = 0, a(1) =
 *            2, phi = (1+sqrt(5))/2 (the listed terms need phi, not
 *            (sqrt(5)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* floor(phi*x) by Zeckendorf shift: sum F_{k+1} over the terms F_k of x,
   minus 1 if the smallest index k is even. Uses only values <= ~phi*x. */
static seq_t floor_phi_fixed(seq_t x) {
    seq_t f[200];
    int m, i, low = 0;
    seq_t r, sigma = 0;
    if (x == 0) return 0;
    f[0] = 1; /* F_2 */
    f[1] = 2; /* F_3 */
    m = 1;
    while (f[m] <= x && m < 198) {
        seq_t nx = f[m] + f[m - 1];
        m++;
        f[m] = nx;
        if (nx < f[m - 1]) break; /* overflow */
    }
    r = x;
    for (i = m - 1; i >= 0; i--) {
        if (f[i] <= r) {
            r -= f[i];
            sigma += f[i + 1];
            low = i;
        }
    }
    /* f[i] = F_{i+2}, so index k is even iff i is even */
    return (low % 2 == 0) ? sigma - 1 : sigma;
}

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_cnt = 0;
static int width_mode = -1; /* 1 = fixed width, 0 = arbitrary precision */

static seq_t big_fa, big_fb; /* consecutive Fibonacci numbers F_k, F_{k+1} */
static bool big_init = false;

static bool is_fixed_width(void) {
    if (width_mode < 0) {
        seq_t t = 1;
        int i;
        for (i = 0; i < 200; i++) t *= 2;
        width_mode = (t == 0) ? 1 : 0;
    }
    return width_mode == 1;
}

/* floor(phi*x) as floor(x*F_{k+1}/F_k) with F_k >= 4x, which is exact
   because x*phi is at least ~0.38/x away from any integer. */
static seq_t floor_phi_big(seq_t x) {
    if (x == 0) return 0;
    if (!big_init) {
        big_fa = 1;
        big_fb = 2;
        big_init = true;
    }
    while (big_fa < x * 4) {
        seq_t t = big_fa + big_fb;
        big_fa = big_fb;
        big_fb = t;
    }
    return x * big_fb / big_fa;
}

static seq_t floor_phi(seq_t x) {
    return is_fixed_width() ? floor_phi_fixed(x) : floor_phi_big(x);
}

static void ensure_computed(idx_t up_to) {
    idx_t i;
    if (memo == NULL) {
        memo_cap = (up_to < 256) ? 256 : up_to + 1;
        memo = mem_new(seq_t, memo_cap);
        for (i = 0; i < memo_cap; i++) memo[i] = 0;
        memo[0] = 0;
        memo[1] = 2;
        memo_cnt = 2;
    } else if (up_to >= memo_cap) {
        idx_t new_cap = memo_cap;
        seq_t *nm;
        while (new_cap <= up_to) new_cap *= 2;
        nm = mem_new(seq_t, new_cap);
        for (i = 0; i < new_cap; i++) nm[i] = 0;
        for (i = 0; i < memo_cnt; i++) nm[i] = memo[i];
        mem_free(memo);
        memo = nm;
        memo_cap = new_cap;
    }
    while (memo_cnt <= up_to) {
        idx_t k = memo_cnt;
        memo[k] = floor_phi(memo[k - 1]) + floor_phi(memo[k - 2]);
        memo_cnt++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    ensure_computed(n);
    return memo[n];
}
