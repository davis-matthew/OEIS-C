/*
 * A065644  a(n) is the smallest integer k such that
 *          floor((3/2)^k)/floor((3/2)^n) is an integer greater than 1.
 * https://oeis.org/A065644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n with floor((3/2)^n) dividing floor((3/2)^k);
 *            computed by search, using exact bignum 3^k >> k reduced mod
 *            floor((3/2)^n)
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 u128_t;

static u128_t shift_mod(const uint32_t *P, size_t len, uint64_t k, uint64_t m) {
    size_t s = (size_t)(k / 32);
    unsigned b = (unsigned)(k % 32);
    u128_t rem = 0;
    if (s >= len) return 0;
    size_t j = len - s;
    while (j > 0) {
        j--;
        uint32_t w = P[j + s] >> b;
        if (b && j + s + 1 < len) w |= (uint32_t)(P[j + s + 1] << (32 - b));
        if (m) rem = ((rem << 32) | w) % m;
        else rem = (rem << 32) | w;
    }
    return rem;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    size_t cap = 64, len = 1;
    uint32_t *P = (uint32_t *)malloc(cap * sizeof(uint32_t));
    P[0] = 1;
    uint64_t m = 0;
    uint64_t k = 0;
    while (1) {
        if (k == nn) {
            m = (uint64_t)shift_mod(P, len, k, 0);
        } else if (k > nn) {
            if (shift_mod(P, len, k, m) == 0) break;
        }
        /* P *= 3 */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; i++) {
            uint64_t v = (uint64_t)P[i] * 3 + carry;
            P[i] = (uint32_t)v;
            carry = v >> 32;
        }
        if (carry) {
            if (len == cap) {
                cap *= 2;
                P = (uint32_t *)realloc(P, cap * sizeof(uint32_t));
            }
            P[len++] = (uint32_t)carry;
        }
        k++;
    }
    free(P);
    return (seq_t)k;
}
