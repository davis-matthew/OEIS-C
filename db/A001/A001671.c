/*
 * A001671  Powers of e rounded up.
 * https://oeis.org/A001671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(e^n). Computed exactly with fixed-point binary
 *            arithmetic: e = Sum_{k>=0} 1/k! to about 1.4427*n + 128 fraction
 *            bits, then e^n by binary powering, then floor(e^n) + 1 (e^n is
 *            never an integer for n >= 1), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   a001671_mulfix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001671_mulfix.h"

seq_t a(idx_t n) {
    size_t I, F, L, i;
    uint64_t bits;
    uint32_t *e, *term, *res, *tmp;
    uint64_t k;
    int top, b;
    seq_t r;

    if (n == 0) return (seq_t)1;

    bits = (uint64_t)n * 14427 / 10000;
    I = (size_t)(bits / 32 + 3);
    F = (size_t)((bits + 128) / 32 + 2);
    L = I + F;

    e = (uint32_t *)calloc(L, sizeof(uint32_t));
    term = (uint32_t *)calloc(L, sizeof(uint32_t));
    res = (uint32_t *)calloc(L, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));

    /* e = sum 1/k!, scaled by 2^(32F) */
    term[F] = 1;
    e[F] = 1;
    top = (int)F;
    for (k = 1; top >= 0; k++) {
        uint64_t rem = 0;
        uint64_t carry = 0;
        int t;
        for (t = top; t >= 0; t--) {
            uint64_t cur = (rem << 32) | term[t];
            term[t] = (uint32_t)(cur / k);
            rem = cur % k;
        }
        while (top >= 0 && term[top] == 0) top--;
        if (top < 0) break;
        for (i = 0; i < L; i++) {
            uint64_t s = (uint64_t)e[i] + (i <= (size_t)top ? term[i] : 0) + carry;
            e[i] = (uint32_t)s;
            carry = s >> 32;
            if (i > (size_t)top && carry == 0) break;
        }
    }

    /* res = e^n */
    res[F] = 1;
    b = 63;
    while (b > 0 && !((n >> b) & 1)) b--;
    for (; b >= 0; b--) {
        a001671_mulfix(res, res, res, tmp, L, F);
        if ((n >> b) & 1) a001671_mulfix(res, e, res, tmp, L, F);
    }

    /* integer part, then +1 */
    r = 0;
    for (i = L; i > F; i--) {
        uint32_t limb = res[i - 1];
        int s;
        for (s = 3; s >= 0; s--) {
            r = r * 256;
            r = r + (unsigned)((limb >> (8 * s)) & 255u);
        }
    }
    r = r + 1;

    free(e);
    free(term);
    free(res);
    free(tmp);
    return r;
}
