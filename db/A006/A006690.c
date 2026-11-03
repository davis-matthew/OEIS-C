/*
 * A006690  Number of deterministic, completely-defined, initially-connected
 *          finite automata with 3 inputs and n unlabeled states.
 * https://oeis.org/A006690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum (Product_{i=1..n-1} i^(f_i - f_{i-1} - 1)) * n^(3*n -
 *            f_{n-1} - 1), summed over 0 <= f_1 < 3, f_{i-1} < f_i < 3*i;
 *            evaluated by DP: D_1(f) = 1 for 0 <= f < 3, D_i(f) = i*D_i(f-1)
 *            + D_{i-1}(f-1) for 0 < f < 3*i, D_i(0) = 0, a(n) = n^3 *
 *            Sum_{f=0..3n-4} D_{n-1}(f) * n^(3n-4-f), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n) {
    if (n == 1) return 1;
    idx_t len = 3 * (n - 1);
    seq_t *d = alloc_zero(len + 1);
    for (idx_t f = 0; f < 3; f++) d[f] = 1;
    for (idx_t i = 2; i <= n - 1; i++) {
        idx_t top = 3 * i;
        seq_t prev_old = d[0];   /* D_{i-1}(f-1) */
        d[0] = 0;
        for (idx_t f = 1; f < top; f++) {
            seq_t cur_old = d[f];
            d[f] = (seq_t)i * d[f - 1] + prev_old;
            prev_old = cur_old;
        }
    }
    seq_t s = 0;
    seq_t nn = (seq_t)n;
    for (idx_t f = 0; f < len; f++) {
        s = s * nn + d[f];
    }
    s = s * nn * nn * nn;
    release(d, len + 1);
    return s;
}
