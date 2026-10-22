/*
 * A006840  Number of 2n-bead black-white reversible complementable necklaces
 *          with n black beads.
 * https://oeis.org/A006840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over the group D_{2n} x {identity, complement},
 *            which has 8n elements, with N = 2n: 8n*a(n) = Sum_{d|N, d even}
 *            phi(N/d)*C(d,d/2) + Sum_{d|N, N/d even} phi(N/d)*2^d + n*V + n*E
 *            + n*2^n, where V = C(n,n/2) if n is even, else 2*C(n-1,(n-1)/2);
 *            E = C(n,n/2) if n is even, else 0. a(0...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   mp_binom, mp_div_small, mp_mul_small, mp_pow2, mp_zero,
 *            totient_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mp_binom.h"
#include "../utils/mp_div_small.h"
#include "../utils/mp_mul_small.h"
#include "../utils/mp_pow2.h"
#include "../utils/mp_zero.h"
#include "../utils/totient_u.h"

static void mp_add(uint32_t *acc, const uint32_t *x, size_t L) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)acc[i] + x[i] + carry;
        acc[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t nn = (uint64_t)n;
    uint64_t N = 2 * nn;
    size_t L = (size_t)(N / 32 + 6);
    uint32_t *total = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t d;

    for (d = 1; d <= N; d++) {
        if (N % d != 0) continue;
        uint64_t ph = totient_u(N / d);
        if (d % 2 == 0) {
            mp_binom(tmp, L, d, d / 2);
            mp_mul_small(tmp, L, ph);
            mp_add(total, tmp, L);
        }
        if ((N / d) % 2 == 0) {
            mp_pow2(tmp, L, d);
            mp_mul_small(tmp, L, ph);
            mp_add(total, tmp, L);
        }
    }

    /* reflections without complement */
    if (nn % 2 == 0) {
        mp_binom(tmp, L, nn, nn / 2);
        mp_mul_small(tmp, L, 2 * nn); /* V + E */
        mp_add(total, tmp, L);
    } else {
        mp_binom(tmp, L, nn - 1, (nn - 1) / 2);
        mp_mul_small(tmp, L, 2 * nn); /* V */
        mp_add(total, tmp, L);
    }

    /* reflections with complement */
    mp_pow2(tmp, L, nn);
    mp_mul_small(tmp, L, nn);
    mp_add(total, tmp, L);

    mp_div_small(total, L, 8 * nn);

    size_t top = L;
    while (top > 0 && total[top - 1] == 0) top--;
    seq_t r = 0;
    while (top > 0) {
        top--;
        uint32_t w = total[top];
        r = (seq_t)(r << 16);
        r += (seq_t)(unsigned)(w >> 16);
        r = (seq_t)(r << 16);
        r += (seq_t)(unsigned)(w & 0xFFFFu);
    }
    free(total);
    free(tmp);
    return r;
}
