/*
 * A004571  Expansion of sqrt(7) in base 4.
 * https://oeis.org/A004571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2; for n >= 2, a(n) = 2*b(2n-3) + b(2n-2), where b(k) is
 *            the k-th binary digit of sqrt(7) after the point, computed with
 *            the exact bit-by-bit square root: r(0) = 2, s(0) = 3; if
 *            4*s(k-1) >= 4*r(k-1)+1 then b(k) = 1, r(k) = 2*r(k-1)+1, s(k) =
 *            4*s(k-1)-4*r(k-1)-1, else b(k) = 0, r(k) = 2*r(k...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *g_bits = NULL;
static size_t g_nbits = 0;

static void compute_bits(size_t K) {
    size_t L = K / 32 + 6;
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *bits = (unsigned char *)malloc(K + 1);
    size_t k, i;
    r[0] = 2;
    rem[0] = 3;
    bits[0] = 0;
    for (k = 1; k <= K; k++) {
        size_t len = (k + 8) / 32 + 2;
        uint32_t carry;
        int ge;
        if (len > L) len = L;
        /* rem <<= 2 */
        carry = 0;
        for (i = 0; i < len; i++) {
            uint32_t v = rem[i];
            rem[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        /* t = (r << 2) | 1 */
        carry = 0;
        for (i = 0; i < len; i++) {
            uint32_t v = r[i];
            t[i] = (v << 2) | carry;
            carry = v >> 30;
        }
        t[0] |= 1u;
        /* compare rem >= t */
        ge = 1;
        for (i = len; i-- > 0;) {
            if (rem[i] != t[i]) {
                ge = rem[i] > t[i];
                break;
            }
        }
        if (ge) {
            uint64_t borrow = 0;
            for (i = 0; i < len; i++) {
                uint64_t d = (uint64_t)rem[i] - t[i] - borrow;
                rem[i] = (uint32_t)d;
                borrow = (d >> 63) & 1u;
            }
        }
        /* r = 2r + bit */
        carry = ge ? 1u : 0u;
        for (i = 0; i < len; i++) {
            uint32_t v = r[i];
            r[i] = (v << 1) | carry;
            carry = v >> 31;
        }
        bits[k] = (unsigned char)(ge ? 1 : 0);
    }
    free(r);
    free(rem);
    free(t);
    if (g_bits) free(g_bits);
    g_bits = bits;
    g_nbits = K;
}

seq_t a(idx_t n) {
    size_t K;
    seq_t res;
    if (n <= 1) return (seq_t)2;
    K = (size_t)(2 * n - 2);
    if (g_nbits < K) {
        size_t want = 2 * g_nbits;
        if (want < K) want = K;
        compute_bits(want);
    }
    res = (seq_t)(2 * g_bits[2 * n - 3] + g_bits[2 * n - 2]);
    return res;
}
