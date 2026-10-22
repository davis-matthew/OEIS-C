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
 * Helpers:   a005648_addmul, a005648_div, a005648_mul, a005648_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a005648_addmul.h"
#include "../utils/a005648_div.h"
#include "../utils/a005648_mul.h"
#include "../utils/a005648_phi.h"

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
            a005648_addmul(s, &slen, c, clen, a005648_phi(n / d));
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
