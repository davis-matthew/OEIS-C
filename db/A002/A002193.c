/*
 * A002193  Decimal expansion of square root of 2.
 * https://oeis.org/A002193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   sqrt(2) = (7/5) * Sum_{k>=0} binomial(2k,k)/200^k; a(n) = n-th
 *            decimal digit of sqrt(2), computed with base-10^9 fixed-point
 *            arithmetic
 * Range:     u8 n <= 9724, u16 n <= 9724, u32 n <= 9724, u64 n <= 9724,
 *            u128 n <= 9724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *s_dig = NULL;
static idx_t s_cap = 0;

static void sqrt2_compute(idx_t D) {
    size_t L = 1 + (size_t)((D + 8) / 9) + 4;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    const uint64_t B = 1000000000ULL;
    size_t i;
    uint64_t k;
    t[0] = 1;
    s[0] = 1;
    for (k = 1;; k++) {
        uint64_t carry = 0, rem = 0, m = 2 * k - 1, d = 100 * k;
        int nz = 0;
        for (i = L; i-- > 0;) {
            uint64_t v = (uint64_t)t[i] * m + carry;
            t[i] = (uint32_t)(v % B);
            carry = v / B;
        }
        for (i = 0; i < L; i++) {
            uint64_t v = rem * B + t[i];
            t[i] = (uint32_t)(v / d);
            rem = v % d;
            if (t[i]) nz = 1;
        }
        if (!nz) break;
        carry = 0;
        for (i = L; i-- > 0;) {
            uint64_t v = (uint64_t)s[i] + t[i] + carry;
            s[i] = (uint32_t)(v % B);
            carry = v / B;
        }
    }
    {
        uint64_t carry = 0, rem = 0;
        for (i = L; i-- > 0;) {
            uint64_t v = (uint64_t)s[i] * 7 + carry;
            s[i] = (uint32_t)(v % B);
            carry = v / B;
        }
        for (i = 0; i < L; i++) {
            uint64_t v = rem * B + s[i];
            s[i] = (uint32_t)(v / 5);
            rem = v % 5;
        }
    }
    free(s_dig);
    s_dig = (unsigned char *)malloc((size_t)D + 1);
    s_dig[1] = (unsigned char)(s[0] % 10);
    {
        idx_t n;
        for (n = 2; n <= D; n++) {
            idx_t p = n - 2;
            uint32_t limb = s[1 + p / 9];
            int pos = (int)(p % 9);
            int j;
            for (j = 0; j < 8 - pos; j++) limb /= 10;
            s_dig[n] = (unsigned char)(limb % 10);
        }
    }
    s_cap = D;
    free(t);
    free(s);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n > s_cap) {
        idx_t D = n * 2;
        if (D < 1024) D = 1024;
        sqrt2_compute(D);
    }
    return (seq_t)(unsigned)s_dig[n];
}
