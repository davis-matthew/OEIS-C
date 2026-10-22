/*
 * A004569  Expansion of sqrt(7) in base 2.
 * https://oeis.org/A004569
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(7) * 2^(n-3)) mod 2, i.e. the binary digits of
 *            sqrt(7) = 10.1010010...; computed with the exact bit-by-bit
 *            integer square root of 7*4^k on multiword integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq7_cmp, sq7_shl, sq7_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/sq7_cmp.h"
#include "../utils/sq7_shl.h"
#include "../utils/sq7_sub.h"

static unsigned char *sq7_bits = NULL;
static idx_t sq7_nbits = 0;

static void sq7_compute(idx_t N) {
    size_t W = (size_t)(N / 32) + 4;
    uint32_t *y = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(W, sizeof(uint32_t));
    unsigned char *b = (unsigned char *)malloc((size_t)N);
    idx_t i;
    /* y_0 = floor(sqrt(7)) = 2, r_0 = 7 - 4 = 3 */
    y[0] = 2;
    r[0] = 3;
    b[0] = 1;
    b[1] = 0;
    for (i = 2; i < N; i++) {
        int bit = 0;
        sq7_shl(r, W, 2);
        memcpy(t, y, W * sizeof(uint32_t));
        sq7_shl(t, W, 2);
        t[0] |= 1;
        if (sq7_cmp(r, t, W) >= 0) {
            sq7_sub(r, t, W);
            bit = 1;
        }
        sq7_shl(y, W, 1);
        y[0] |= (uint32_t)bit;
        b[i] = (unsigned char)bit;
    }
    free(y);
    free(r);
    free(t);
    free(sq7_bits);
    sq7_bits = b;
    sq7_nbits = N;
}

seq_t a(idx_t n) {
    idx_t k;
    if (n < 2) return 0;
    k = n - 2;
    if (k >= sq7_nbits) {
        idx_t N = sq7_nbits * 2;
        if (N < k + 1) N = k + 1;
        if (N < 64) N = 64;
        sq7_compute(N);
    }
    return (seq_t)sq7_bits[k];
}
