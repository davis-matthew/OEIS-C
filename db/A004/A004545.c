/*
 * A004545  Expansion of sqrt(2) in base 8.
 * https://oeis.org/A004545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(1) = 1 (integer part); for n >= 2, a(n) is the n-th base-8
 *            digit of sqrt(2) = floor(sqrt(2)*8^(n-1)) mod 8. Computed
 *            exactly by the binary digit-by-digit square root, using native
 *            multiword integers; results are memoized
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq2_cmp, sq2_shl, sq2_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sq2_cmp.h"
#include "../utils/sq2_shl.h"
#include "../utils/sq2_sub.h"

static unsigned char *sq2_bits = NULL;
static idx_t sq2_nbits = 0;

static void sq2_compute(idx_t nbits) {
    size_t L = (size_t)(nbits / 32) + 4;
    uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *bits = (unsigned char *)malloc((size_t)nbits);
    idx_t i;
    for (i = 0; i < nbits; i++) {
        size_t act = (size_t)((i + 4) / 32) + 2;
        if (act > L) act = L;
        sq2_shl(R, act, 2);
        if (i == 0) R[0] |= 2u;
        memcpy(T, Y, act * sizeof(uint32_t));
        sq2_shl(T, act, 2);
        T[0] |= 1u;
        sq2_shl(Y, act, 1);
        if (sq2_cmp(R, T, act) >= 0) {
            sq2_sub(R, T, act);
            Y[0] |= 1u;
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
    free(R);
    free(Y);
    free(T);
    free(sq2_bits);
    sq2_bits = bits;
    sq2_nbits = nbits;
}

seq_t a(idx_t n) {
    idx_t need, k;
    int d;
    if (n <= 1) return (seq_t)1;
    need = 3 * (n - 1) + 1;
    if (need > sq2_nbits) {
        idx_t target = need * 2;
        if (target < 256) target = 256;
        sq2_compute(target);
    }
    k = 3 * n - 5;
    d = (sq2_bits[k] << 2) | (sq2_bits[k + 1] << 1) | sq2_bits[k + 2];
    return (seq_t)d;
}
