/*
 * A006789  Bessel numbers: the number of nonoverlapping partitions of an
 *          n-set into equivalence classes.
 * https://oeis.org/A006789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = weighted Motzkin-path count from the J-fraction G.f. 1/(1
 *            - x - x^2/(1 - 2*x - x^2/(1 - 3*x - ...))): f_0(0) = 1,
 *            f_{i+1}(h) = f_i(h-1) + (h+1)*f_i(h) + f_i(h+1), a(n) = f_n(0)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 631 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n) {
    static seq_t *f = NULL;   /* f[h] = weighted paths of 'done-1' steps ending at height h */
    static idx_t fcap = 0;
    static seq_t *v = NULL;   /* v[i] = a(i) for i < done */
    static idx_t vcap = 0;
    static idx_t done = 0;

    if (f == NULL) {
        fcap = 64;
        f = alloc_zero(fcap);
        f[0] = 1;
        vcap = 64;
        v = alloc_zero(vcap);
        v[0] = 1;
        done = 1;
    }

    while (n >= done) {
        idx_t step = done - 1;
        if (step + 3 > fcap) {
            idx_t nc = fcap;
            while (step + 3 > nc) nc *= 2;
            f = grow_zero(f, fcap, nc);
            fcap = nc;
        }
        seq_t prev = 0;
        for (idx_t h = 0; h <= step + 1; h++) {
            seq_t cur = f[h];
            f[h] = prev + (seq_t)(h + 1) * cur + f[h + 1];
            prev = cur;
        }
        if (done >= vcap) {
            idx_t nc = vcap * 2;
            v = grow_zero(v, vcap, nc);
            vcap = nc;
        }
        v[done] = f[0];
        done++;
    }

    return v[n];
}
