/*
 * A005045  Number of restricted 3 X 3 matrices with row and column sums n.
 * https://oeis.org/A005045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: x^2*(1+x-x^3-x^5+x^6)/((1-x)^2*(1-x^2)*(1-x^3)*(1-x^4)),
 *            i.e. with S(m) = [x^m] 1/((1-x)^2(1-x^2)(1-x^3)(1-x^4)) (S(m)=0
 *            for m<0): a(n) = S(n-2) + S(n-3) - S(n-5) - S(n-7) + S(n-8)
 * Range:     u8 n <= 15, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 979 (2026-11-03)
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

static seq_t a005045_get(seq_t *s, idx_t n, idx_t d) {
    if (n < d) return 0;
    return s[n - d];
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    idx_t len = n - 1; /* need S[0..n-2] */
    seq_t *s = alloc_zero(len);
    idx_t i;
    /* 1/(1-x)^2 */
    for (i = 0; i < len; i++) s[i] = (seq_t)i + 1;
    /* 1/(1-x^2), 1/(1-x^3), 1/(1-x^4) */
    {
        idx_t k;
        for (k = 2; k <= 4; k++)
            for (i = k; i < len; i++) s[i] += s[i - k];
    }
    seq_t pos = a005045_get(s, n, 2) + a005045_get(s, n, 3) + a005045_get(s, n, 8);
    seq_t neg = a005045_get(s, n, 5) + a005045_get(s, n, 7);
    seq_t r = pos - neg;
    release(s, len);
    return r;
}
