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
 * Helpers:   floor_phi_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_phi_fixed.h"

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
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        for (i = 0; i < memo_cap; i++) memo[i] = 0;
        memo[0] = 0;
        memo[1] = 2;
        memo_cnt = 2;
    } else if (up_to >= memo_cap) {
        idx_t new_cap = memo_cap;
        seq_t *nm;
        while (new_cap <= up_to) new_cap *= 2;
        nm = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (i = 0; i < new_cap; i++) nm[i] = 0;
        for (i = 0; i < memo_cnt; i++) nm[i] = memo[i];
        free(memo);
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
