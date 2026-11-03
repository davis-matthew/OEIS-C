/*
 * A001895  Number of rooted planar 2-trees with n nodes.
 * https://oeis.org/A001895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   from the g.f.
 *            (4-8*x^2-sqrt(1-4*x)-(3+2*x)*sqrt(1-4*x^2))/(8*x^2), with
 *            Catalan numbers C(k) = binomial(2k,k)/(k+1): a(n) = (C(n+1) +
 *            3*C(n/2))/4 for n even, a(n) = (C(n+1) + 2*C((n-1)/2))/4 for n
 *            odd. The Catalan numbers are computed exactly in 32-bit limbs
 *            and the result is then converted to seq_t
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 188 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void a1895_divsm(uint32_t *x, size_t *len, uint32_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = *len; i-- > 0;) {
        r = (r << 32) | x[i];
        x[i] = (uint32_t)(r / d);
        r %= d;
    }
    while (*len > 1 && x[*len - 1] == 0) (*len)--;
}

static void a1895_mulsm(uint32_t *x, size_t *len, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) {
        x[(*len)++] = (uint32_t)c;
        c >>= 32;
    }
}

static void a1895_catalan(idx_t k, uint32_t *x, size_t *len) {
    idx_t j;
    x[0] = 1;
    *len = 1;
    for (j = 0; j < k; j++) {
        a1895_mulsm(x, len, (uint32_t)(2 * (2 * j + 1)));
        a1895_divsm(x, len, (uint32_t)(j + 2));
    }
}

seq_t a(idx_t n) {
    idx_t K = n + 1, m;
    uint32_t c;
    size_t cap, la, lb, i;
    uint32_t *A, *B;
    uint64_t carry;
    seq_t res = 0;

    if (n % 2 == 0) { m = n / 2; c = 3; }
    else { m = (n - 1) / 2; c = 2; }

    cap = (size_t)((2 * K + 1) / 32 + 4);
    A = (uint32_t *)calloc(cap, sizeof(uint32_t));
    B = (uint32_t *)calloc(cap, sizeof(uint32_t));

    a1895_catalan(K, A, &la);
    a1895_catalan(m, B, &lb);
    a1895_mulsm(B, &lb, c);

    /* A += B */
    carry = 0;
    for (i = 0; i < la || i < lb || carry; i++) {
        uint64_t s = carry + (i < la ? A[i] : 0) + (i < lb ? B[i] : 0);
        A[i] = (uint32_t)s;
        carry = s >> 32;
    }
    if (i > la) la = i;

    /* A >>= 2 */
    for (i = 0; i < la; i++) {
        uint32_t hi = (i + 1 < la) ? A[i + 1] : 0;
        A[i] = (A[i] >> 2) | (hi << 30);
    }
    while (la > 1 && A[la - 1] == 0) la--;

    /* convert to seq_t, byte by byte from the top */
    for (i = la; i-- > 0;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            res = (seq_t)(res * 256);
            res += (idx_t)((A[i] >> sh) & 0xFFu);
        }
    }

    free(A);
    free(B);
    return res;
}
