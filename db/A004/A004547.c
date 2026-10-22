/*
 * A004547  Expansion of sqrt(3) in base 2.
 * https://oeis.org/A004547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(3) * 2^(n-1)) mod 2, i.e. the n-th bit of the
 *            root produced by the binary digit-by-digit square root of 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *a004547_bits = 0;
static idx_t a004547_done = 0;

static void a004547_compute(idx_t L) {
    size_t W = (size_t)(L / 32 + 4);
    uint32_t *r = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(W, sizeof(uint32_t));
    unsigned char *bits = (unsigned char *)calloc((size_t)L + 1, 1);
    idx_t k;
    size_t i;
    for (k = 1; k <= L; k++) {
        uint32_t p = (k == 1) ? 3u : 0u;
        uint32_t carry;
        int ge;
        /* r = 4r + p */
        carry = p;
        for (i = 0; i < W; i++) {
            uint32_t nc = r[i] >> 30;
            r[i] = (r[i] << 2) | carry;
            carry = nc;
        }
        /* t = 4y + 1 */
        carry = 1;
        for (i = 0; i < W; i++) {
            uint32_t nc = y[i] >> 30;
            t[i] = (y[i] << 2) | carry;
            carry = nc;
        }
        /* compare r >= t */
        ge = 1;
        for (i = W; i-- > 0;) {
            if (r[i] != t[i]) {
                ge = r[i] > t[i];
                break;
            }
        }
        if (ge) {
            uint32_t borrow = 0;
            for (i = 0; i < W; i++) {
                uint64_t d = (uint64_t)r[i] - t[i] - borrow;
                r[i] = (uint32_t)d;
                borrow = (uint32_t)((d >> 63) & 1);
            }
        }
        /* y = 2y + ge */
        carry = (uint32_t)ge;
        for (i = 0; i < W; i++) {
            uint32_t nc = y[i] >> 31;
            y[i] = (y[i] << 1) | carry;
            carry = nc;
        }
        bits[k] = (unsigned char)ge;
    }
    free(r);
    free(y);
    free(t);
    free(a004547_bits);
    a004547_bits = bits;
    a004547_done = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > a004547_done) {
        idx_t L = a004547_done * 2;
        if (L < n) L = n;
        if (L < 64) L = 64;
        a004547_compute(L);
    }
    return (seq_t)a004547_bits[n];
}
