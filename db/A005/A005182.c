/*
 * A005182  a(n) = floor(e^((n-1)/2)).
 * https://oeis.org/A005182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(e^((n-1)/2)), via the series e^x = Sum_{k>=0}
 *            x^k/k! with x = (n-1)/2, in exact multiword fixed point
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t m = (uint64_t)(n - 1);
    /* e^(m/2) has about m/(2 ln 2) < 3m/4 + 1 bits */
    size_t I = (size_t)((3 * m / 4) / 32 + 2);   /* integer words */
    size_t F = I + 3;                             /* fractional words */
    size_t W = I + F;
    uint32_t *t = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(W, sizeof(uint32_t));
    t[F] = 1;
    s[F] = 1;
    for (uint64_t k = 1;; k++) {
        /* t *= m */
        uint64_t carry = 0;
        for (size_t i = 0; i < W; i++) {
            uint64_t cur = (uint64_t)t[i] * m + carry;
            t[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
        /* t /= 2k */
        uint64_t d = 2 * k, rem = 0;
        int nonzero = 0;
        for (size_t i = W; i-- > 0;) {
            uint64_t cur = (rem << 32) | t[i];
            t[i] = (uint32_t)(cur / d);
            rem = cur % d;
            if (t[i]) nonzero = 1;
        }
        if (!nonzero) break;
        /* s += t */
        carry = 0;
        for (size_t i = 0; i < W; i++) {
            uint64_t cur = (uint64_t)s[i] + t[i] + carry;
            s[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    seq_t r = 0;
    for (size_t i = W; i-- > F;) {
        for (int b = 3; b >= 0; b--) {
            r = r * 256 + (int)((s[i] >> (8 * b)) & 255u);
        }
    }
    free(t);
    free(s);
    return r;
}
