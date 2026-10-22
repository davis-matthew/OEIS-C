/*
 * A004609  Expansion of sqrt(6) in base 2.
 * https://oeis.org/A004609
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6)*2^(n-3)) mod 2, computed by digit-by-digit
 *            binary square root (bit k of sqrt(6))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static unsigned char *a004609_bits = NULL;
static idx_t a004609_nbits = 0;

static void a004609_compute(idx_t N) {
    size_t L = (size_t)(N / 32) + 4;
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *b = (unsigned char *)malloc((size_t)N + 1);
    idx_t k;
    size_t i;
    memset(b, 0, (size_t)N + 1);
    /* y = floor(sqrt(6)) = 2, r = 6 - y^2 = 2 */
    y[0] = 2;
    r[0] = 2;
    b[2] = 1;
    if (N >= 3) b[3] = 0;
    for (k = 4; k <= N; k++) {
        uint32_t carry = 0;
        int ge = 1;
        /* r <<= 2 */
        for (i = 0; i < L; i++) {
            uint32_t v = r[i];
            r[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        /* t = 4y + 1 */
        carry = 0;
        for (i = 0; i < L; i++) {
            uint32_t v = y[i];
            t[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        t[0] |= 1;
        /* compare r >= t */
        i = L;
        while (i-- > 0) {
            if (r[i] != t[i]) {
                ge = r[i] > t[i];
                break;
            }
        }
        if (ge) {
            uint64_t borrow = 0;
            for (i = 0; i < L; i++) {
                uint64_t d = (uint64_t)r[i] - (uint64_t)t[i] - borrow;
                r[i] = (uint32_t)d;
                borrow = (d >> 63) & 1;
            }
        }
        /* y = 2y + ge */
        carry = (uint32_t)ge;
        for (i = 0; i < L; i++) {
            uint32_t v = y[i];
            y[i] = (v << 1) | carry;
            carry = v >> 31;
        }
        b[k] = (unsigned char)ge;
    }
    free(y);
    free(r);
    free(t);
    if (a004609_bits) free(a004609_bits);
    a004609_bits = b;
    a004609_nbits = N + 1;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;
    if (n >= a004609_nbits) {
        idx_t N = n;
        if (N < 2 * a004609_nbits) N = 2 * a004609_nbits;
        if (N < 64) N = 64;
        a004609_compute(N);
    }
    return (seq_t)a004609_bits[n];
}
