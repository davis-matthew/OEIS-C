/*
 * A007404  Decimal expansion of Sum_{n>=0} 1/2^(2^n).
 * https://oeis.org/A007404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) * Sum_{k>=0} 1/2^(2^k)) mod 10. Computed
 *            by building the constant as an exact binary fraction (bits at
 *            positions 2^k) to well beyond the needed precision, then
 *            repeatedly multiplying by 10 and taking the integer carry as the
 *            next digit
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 8374 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int digit_with_bits(idx_t n, uint64_t L, int *ok) {
    uint64_t W = (L + 31) / 32;
    uint32_t *f = (uint32_t *)calloc((size_t)W, sizeof(uint32_t));
    uint64_t p, i, it;
    uint64_t carry = 0;
    int d = 0;
    for (p = 1; p <= W * 32; p <<= 1) {
        uint64_t q = p - 1;
        f[q / 32] |= (uint32_t)1u << (31 - (q % 32));
    }
    for (it = 0; it <= (uint64_t)n; it++) {
        carry = 0;
        for (i = W; i-- > 0;) {
            uint64_t v = (uint64_t)f[i] * 10u + carry;
            f[i] = (uint32_t)v;
            carry = v >> 32;
        }
    }
    d = (int)carry;
    /* remaining fraction plus truncation error must not reach 1 */
    *ok = !(W >= 2 && f[0] == 0xFFFFFFFFu && f[1] == 0xFFFFFFFFu);
    free(f);
    return d;
}

seq_t a(idx_t n) {
    uint64_t L = 4 * ((uint64_t)n + 1) + 256;
    int ok = 0;
    int d;
    for (;;) {
        d = digit_with_bits(n, L, &ok);
        if (ok) break;
        L *= 2;
    }
    return (seq_t)d;
}
