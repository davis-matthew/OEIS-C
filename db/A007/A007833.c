/*
 * A007833  Number of point-labeled reduced two-graphs with n nodes.
 * https://oeis.org/A007833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} s(n, k) * 2^((k-1)*(k-2)/2), where s(n, k)
 *            are the signed Stirling numbers of the first kind
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 80 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"
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
    idx_t i, k;
    seq_t *c = alloc_zero(n + 1);
    seq_t pos = 0, neg = 0;

    /* Unsigned Stirling numbers of the first kind |s(i, k)|, row by row */
    c[0] = 1;
    for (i = 1; i <= n; i++) {
        for (k = i; k >= 1; k--) {
            c[k] = c[k - 1] + (seq_t)(i - 1) * c[k];
        }
        c[0] = 0;
    }

    for (k = 1; k <= n; k++) {
        idx_t e = (k == 1) ? 0 : (k - 1) * (k - 2) / 2;
        seq_t term = c[k] * pow2(e);
        if ((n - k) % 2 == 0)
            pos += term;
        else
            neg += term;
    }

    release(c, n + 1);
    return pos - neg;
}
