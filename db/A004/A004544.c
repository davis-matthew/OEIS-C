/*
 * A004544  Expansion of sqrt(2) in base 7.
 * https://oeis.org/A004544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(2) * 7^(n-1)) mod 7; the base-7 digits of
 *            sqrt(2) are found with the digit-by-digit square root method on
 *            multi-limb integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *dig = NULL;
static idx_t ndig = 0;

static int cmp_limbs(const uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
    size_t j;
    if (xl != yl) return xl < yl ? -1 : 1;
    for (j = xl; j > 0; j--) {
        if (x[j - 1] != y[j - 1]) return x[j - 1] < y[j - 1] ? -1 : 1;
    }
    return 0;
}

static void compute_digits(idx_t N) {
    size_t L = (size_t)(N / 8 + 16);
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *nd = (uint8_t *)malloc((size_t)N);
    size_t pl = 0, rl = 0, ql = 0, j;
    idx_t i;
    for (i = 0; i < N; i++) {
        uint64_t carry, v;
        int d;
        /* r = r*49 + next digit pair (2 for the first pair, then 0) */
        carry = (i == 0) ? 2 : 0;
        for (j = 0; j < rl; j++) {
            v = (uint64_t)r[j] * 49 + carry;
            r[j] = (uint32_t)v;
            carry = v >> 32;
        }
        while (carry) { r[rl++] = (uint32_t)carry; carry >>= 32; }
        /* largest d with (14p + d)*d <= r */
        for (d = 6; d > 0; d--) {
            carry = (uint64_t)(d * d);
            for (j = 0; j < pl; j++) {
                v = (uint64_t)p[j] * (uint64_t)(14 * d) + carry;
                q[j] = (uint32_t)v;
                carry = v >> 32;
            }
            ql = pl;
            while (carry) { q[ql++] = (uint32_t)carry; carry >>= 32; }
            if (cmp_limbs(q, ql, r, rl) <= 0) break;
        }
        if (d > 0) {
            uint64_t borrow = 0;
            for (j = 0; j < rl; j++) {
                uint64_t sub = (j < ql ? (uint64_t)q[j] : 0) + borrow;
                uint64_t cur = (uint64_t)r[j];
                if (cur >= sub) { r[j] = (uint32_t)(cur - sub); borrow = 0; }
                else { r[j] = (uint32_t)(cur + ((uint64_t)1 << 32) - sub); borrow = 1; }
            }
            while (rl > 0 && r[rl - 1] == 0) rl--;
        }
        /* p = p*7 + d */
        carry = (uint64_t)d;
        for (j = 0; j < pl; j++) {
            v = (uint64_t)p[j] * 7 + carry;
            p[j] = (uint32_t)v;
            carry = v >> 32;
        }
        while (carry) { p[pl++] = (uint32_t)carry; carry >>= 32; }
        nd[i] = (uint8_t)d;
    }
    free(p); free(r); free(q);
    free(dig);
    dig = nd;
    ndig = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > ndig) {
        idx_t N = n;
        if (N < 2 * ndig) N = 2 * ndig;
        if (N < 64) N = 64;
        compute_digits(N);
    }
    return (seq_t)dig[n - 1];
}
