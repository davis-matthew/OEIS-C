/*
 * A005648  Number of 2n-bead black-white reversible necklaces with n black
 *          beads.
 * https://oeis.org/A005648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = ( Sum_{d|n} phi(n/d)*C(2*d, d) )/(4*n) + C(2*k, k)/2,
 *            where k = floor(n/2); a(0) = 1. Computed as (Sum_{d|n}
 *            phi(n/d)*C(2d,d)/(2n) + C(2k,k))/2 using an internal limb
 *            bignum, so intermediates never overflow seq_t
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 1660 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/totient_u64.h"

static void a005648_addmul(uint32_t *s, size_t *slen, const uint32_t *c, size_t clen, uint64_t m) {
    uint64_t carry = 0;
    size_t i = 0;
    for (; i < clen; i++) {
        uint64_t si = (i < *slen) ? s[i] : 0;
        uint64_t t = (uint64_t)c[i] * m + si + carry;
        s[i] = (uint32_t)t;
        carry = t >> 32;
    }
    for (; carry || i < *slen; i++) {
        uint64_t si = (i < *slen) ? s[i] : 0;
        uint64_t t = si + carry;
        s[i] = (uint32_t)t;
        carry = t >> 32;
        if (i >= *slen && !carry) { i++; break; }
    }
    if (i > *slen) *slen = i;
    while (*slen > 1 && s[*slen - 1] == 0) (*slen)--;
}

static void a005648_div(uint32_t *c, size_t *len, uint64_t x) {
    uint64_t rem = 0;
    for (size_t i = *len; i-- > 0;) {
        uint64_t cur = (rem << 32) | c[i];
        c[i] = (uint32_t)(cur / x);
        rem = cur % x;
    }
    while (*len > 1 && c[*len - 1] == 0) (*len)--;
}

static void a005648_mul(uint32_t *c, size_t *len, uint64_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)c[i] * m + carry;
        c[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry) {
        c[(*len)++] = (uint32_t)carry;
        carry >>= 32;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t k = n / 2;
    size_t cap = (size_t)(n / 16) + 16;
    uint32_t *c = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *b = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t clen = 1, slen = 1, blen = 1;
    c[0] = 1; s[0] = 0; b[0] = 1;

    for (idx_t d = 1; d <= n; d++) {
        /* C(2d,d) = C(2d-2,d-1) * 2(2d-1) / d */
        a005648_mul(c, &clen, 2 * (2 * (uint64_t)d - 1));
        a005648_div(c, &clen, (uint64_t)d);
        if (d == k) {
            memcpy(b, c, clen * sizeof(uint32_t));
            blen = clen;
        }
        if (n % d == 0)
            a005648_addmul(s, &slen, c, clen, totient_u64(n / d));
    }

    a005648_div(s, &slen, 2 * (uint64_t)n);
    a005648_addmul(s, &slen, b, blen, 1);
    a005648_div(s, &slen, 2);

    seq_t r = 0;
    for (size_t i = slen; i-- > 0;) {
        r = r * 65536 + (idx_t)(s[i] >> 16);
        r = r * 65536 + (idx_t)(s[i] & 0xFFFFu);
    }
    free(c);
    free(s);
    free(b);
    return r;
}
