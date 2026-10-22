/*
 * A003169  Number of 2-line arrays; or number of P-graphs with 2n edges.
 * https://oeis.org/A003169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for exact (big-integer) values, a(n) =
 *            ((324*n^2-708*n+360)*a(n-1) - (371*n^2-1831*n+2250)*a(n-2) +
 *            (20*n^2-130*n+210)*a(n-3)) / (16*n*(2*n-1)) for n > 2, a(0) = 0,
 *            a(1) = 1, a(2) = 3. For fixed-width types: from A =
 *            x*(1+A)/(1-A)^2, i.e. A - 2*A^2 + A^3 = x + x*A, a(n) = [n=1] +
 *            a(n-1) + 2*[x^n]...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   A_grow, a003169_fixed, a003169_fixed_calc, a003169_from128,
 *            alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/A_grow.h"
#include "../utils/a003169_fixed.h"
#include "../utils/a003169_fixed_calc.h"
#include "../utils/a003169_from128.h"

static seq_t a003169_big_calc(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0, done = 2;
    idx_t i;
    if (cache == NULL) {
        cap = (n < 100) ? 100 : n + 10;
        cache = alloc_zero(cap);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
    }
    if (n >= cap) {
        cache = A_grow(cache, cap, n + 10);
        cap = n + 10;
    }
    for (i = done + 1; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t n2 = ni * ni;
        seq_t c1 = (seq_t)324 * n2 + (seq_t)360 - (seq_t)708 * ni;
        seq_t c2 = (seq_t)371 * n2 + (seq_t)2250 - (seq_t)1831 * ni;
        seq_t c3 = (seq_t)20 * n2 + (seq_t)210 - (seq_t)130 * ni;
        seq_t den = (seq_t)16 * ni * ((seq_t)2 * ni - 1);
        seq_t num = c1 * cache[i - 1] + c3 * cache[i - 3];
        num -= c2 * cache[i - 2];
        cache[i] = num / den;
    }
    if (n > done) done = n;
    return cache[n];
}

seq_t a(idx_t n) {
    if (a003169_fixed()) return a003169_fixed_calc(n);
    return a003169_big_calc(n);
}
