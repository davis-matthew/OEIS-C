/*
 * A002379  a(n) = floor(3^n / 2^n).
 * https://oeis.org/A002379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3^n / 2^n), i.e. 3^n computed exactly in base-2^32
 *            limbs, then shifted right by n bits
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    size_t cap = (size_t)(n / 20) + 3;
    uint32_t *x = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t len = 1;
    idx_t e = n;
    seq_t res = 0;
    size_t B, nchunks, j;

    x[0] = 1;
    while (e > 0) {
        uint64_t m = 1, carry = 0;
        idx_t k = e >= 20 ? 20 : e;
        idx_t t;
        size_t i;
        for (t = 0; t < k; t++) m *= 3;
        e -= k;
        for (i = 0; i < len; i++) {
            uint64_t v = (uint64_t)x[i] * m + carry;
            x[i] = (uint32_t)v;
            carry = v >> 32;
        }
        if (carry) x[len++] = (uint32_t)carry;
    }

    B = len * 32;
    if ((size_t)n >= B) { free(x); return res; }
    nchunks = (B - (size_t)n + 15) / 16;
    for (j = nchunks; j-- > 0;) {
        size_t p = (size_t)n + 16 * j;
        uint32_t chunk = 0;
        if (p < B) {
            size_t w = p / 32;
            unsigned off = (unsigned)(p % 32);
            uint64_t v = x[w] >> off;
            if (off && w + 1 < len) v |= (uint64_t)x[w + 1] << (32 - off);
            chunk = (uint32_t)(v & 0xFFFFu);
        }
        res = (seq_t)(res * 65536u);
        res += chunk;
    }
    free(x);
    return res;
}
