/*
 * A006251  Number of n-element posets which are unions of 2 chains.
 * https://oeis.org/A006251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f. 4/(2-2*x+sqrt(1-4*x)+sqrt(1-4*x^2)) = (1 +
 *            (1-sqrt(1-4x^2))*(1-2x-sqrt(1-4x))/(4x^2))/(1-x), hence a(n) = 1
 *            + Sum_{j>=1, 2j<=n} C(j-1) * (C(1)+C(2)+...+C(n+1-2j)), C =
 *            Catalan numbers A000108
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *cat = NULL;   /* Catalan numbers C(i) */
static seq_t *csum = NULL;  /* S(i) = C(1)+...+C(i) */
static idx_t have = 0, cap = 0;

static uint64_t small_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void ensure(idx_t k) {
    if (k < have) return;
    if (k + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < k + 1) nc = k + 1;
        if (nc < 64) nc = 64;
        cat = grow_zero(cat, cap, nc);
        csum = grow_zero(csum, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        cat[0] = 1;
        csum[0] = 0;
        have = 1;
    }
    while (have <= k) {
        idx_t i = have - 1;
        uint64_t d = (uint64_t)i + 2;
        uint64_t r = (uint64_t)(cat[i] % d);
        uint64_t g = small_gcd(d, r);
        uint64_t dp = d / g;
        uint64_t m = (2 * (2 * (uint64_t)i + 1)) / dp;
        /* C(i+1) = C(i)*2(2i+1)/(i+2), done without oversized intermediates */
        cat[have] = (cat[i] / g) * m;
        csum[have] = csum[have - 1] + cat[have];
        have++;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)1;
    ensure(n);
    seq_t r = 1;
    idx_t j;
    for (j = 1; 2 * j <= n; j++) {
        r += cat[j - 1] * csum[n + 1 - 2 * j];
    }
    return r;
}
