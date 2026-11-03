/*
 * A005783  Number of 3-covers of an unlabeled n-set.
 * https://oeis.org/A005783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: (x^6+x^4+2*x^3+x^2+1)/((1-x^3)^2*(1-x^2)^2*(1-x)^3)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 159, u64 n <= 6556,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9435 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    static const int steps[7] = {1, 1, 1, 2, 2, 3, 3};
    idx_t len = n + 1;
    seq_t *c = alloc_zero(len);
    c[0] = 1;
    for (int k = 0; k < 7; k++) {
        idx_t s = (idx_t)steps[k];
        for (idx_t i = s; i <= n; i++)
            c[i] += c[i - s];
    }
    seq_t r = c[n];
    if (n >= 2) r += c[n - 2];
    if (n >= 3) { r += c[n - 3]; r += c[n - 3]; }
    if (n >= 4) r += c[n - 4];
    if (n >= 6) r += c[n - 6];
    release(c, len);
    return r;
}
