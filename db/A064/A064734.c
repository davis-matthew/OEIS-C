/*
 * A064734  Final digits of A005165(2n+1) for large n, read from right.
 * https://oeis.org/A064734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(S / 10^n) mod 10, where S = Sum_{k>=1} (-1)^(k+1) *
 *            k! mod 10^(n+1); the sum is finite because k! == 0 (mod
 *            10^(n+1)) once k >= 5*(n+1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* base-10^4 limbs, little-endian, working modulo 10^(4L) */
    idx_t L = n / 4 + 1;
    idx_t K = 5 * (4 * L) + 5;   /* beyond this, k! == 0 mod 10^(4L) */
    uint32_t *f = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)L);
    uint32_t *S = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)L);
    idx_t i, k;
    for (i = 0; i < L; i++) { f[i] = 0; S[i] = 0; }
    f[0] = 1;
    for (k = 1; k <= K; k++) {
        /* f *= k (mod 10^(4L)) */
        uint64_t carry = 0;
        int nonzero = 0;
        for (i = 0; i < L; i++) {
            uint64_t v = (uint64_t)f[i] * (uint64_t)k + carry;
            f[i] = (uint32_t)(v % 10000u);
            carry = v / 10000u;
            if (f[i]) nonzero = 1;
        }
        if (!nonzero) break;
        if (k & 1) {
            uint32_t c = 0;
            for (i = 0; i < L; i++) {
                uint32_t v = S[i] + f[i] + c;
                if (v >= 10000u) { v -= 10000u; c = 1; } else c = 0;
                S[i] = v;
            }
        } else {
            int32_t b = 0;
            for (i = 0; i < L; i++) {
                int32_t v = (int32_t)S[i] - (int32_t)f[i] - b;
                if (v < 0) { v += 10000; b = 1; } else b = 0;
                S[i] = (uint32_t)v;
            }
        }
    }
    uint32_t limb = S[n / 4];
    unsigned r = (unsigned)(n % 4);
    while (r--) limb /= 10u;
    free(f);
    free(S);
    return (seq_t)(limb % 10u);
}
