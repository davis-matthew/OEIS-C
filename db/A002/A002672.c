/*
 * A002672  Denominators of central difference coefficients M_{3}^(2n+1).
 * https://oeis.org/A002672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   (2*sinh(x/2))^3 = 2*sinh(3x/2) - 6*sinh(x/2), so with q = 2n+1,
 *            a(n) = denominator((3^q - 3) / (2^(q-1) * q!)). Computed prime
 *            by prime: v_2(3^q-3) = 2 + v_2(q-1), v_3(3^q-3) = 1, and for
 *            other p with 3^(q-1) == 1 (mod p), v_p(3^q-3) = v_p(3^(p-1)-1) +
 *            v_p(q-1) (LTE)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   a2672_legendre, a2672_mulpow, a2672_powmod, a2672_vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2672_legendre.h"
#include "../utils/a2672_mulpow.h"
#include "../utils/a2672_powmod.h"
#include "../utils/a2672_vp.h"

seq_t a(idx_t n) {
    uint64_t q = 2 * (uint64_t)n + 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)q + 1, 1);
    seq_t res = 1;
    uint64_t chunk = 1;
    uint64_t p;

    for (p = 2; p <= q; p++) {
        uint64_t e, v, k;
        if (comp[p]) continue;
        for (k = p * p; k <= q; k += p) comp[k] = 1;

        if (p == 2) {
            e = (q - 1) + a2672_legendre(q, 2);
            v = 2 + a2672_vp(q - 1, 2);
        } else if (p == 3) {
            e = a2672_legendre(q, 3);
            v = 1;
        } else {
            e = a2672_legendre(q, p);
            if (a2672_powmod(3, q - 1, p) != 1) {
                v = 0;
            } else {
                uint64_t M = p * p * p * p;
                uint64_t r = a2672_powmod(3, p - 1, M);
                uint64_t x = (r + M - 1) % M;
                uint64_t w = 0;
                if (x == 0) {
                    w = 4;
                } else {
                    while (x % p == 0) { x /= p; w++; }
                }
                v = w + a2672_vp(q - 1, p);
            }
        }
        if (v < e) a2672_mulpow(&res, &chunk, p, e - v);
    }
    res *= (seq_t)chunk;
    free(comp);
    return res;
}
