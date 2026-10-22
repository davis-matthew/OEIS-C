/*
 * A004561  Expansion of sqrt(5) in base 8.
 * https://oeis.org/A004561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5)*8^(n-1)) mod 8; uses an exact bit-by-bit
 *            integer square root of 5*4^(3(M-1)), with the result cached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004561_geq, a004561_shl, a004561_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004561_geq.h"
#include "../utils/a004561_shl.h"
#include "../utils/a004561_sub.h"

static unsigned char *a004561_dig = NULL;
static idx_t a004561_ndig = 0;

static void a004561_compute(idx_t M)
{
    size_t S = (size_t)(3 * (M - 1) + 2);
    size_t W = S / 32 + 4;
    uint32_t *q = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(W, sizeof(uint32_t));
    unsigned char *d = (unsigned char *)malloc((size_t)M + 1);
    size_t s, k;
    for (s = 0; s < S; s++) {
        size_t len = s / 32 + 2;
        uint32_t pair = (s < 2) ? 1u : 0u; /* 5 = binary 01 01, then zero pairs */
        if (len > W) len = W;
        a004561_shl(r, len, 2, pair);
        memcpy(t, q, len * sizeof(uint32_t));
        a004561_shl(t, len, 2, 1u);
        if (a004561_geq(r, t, len)) {
            a004561_sub(r, t, len);
            a004561_shl(q, len, 1, 1u);
        } else {
            a004561_shl(q, len, 1, 0u);
        }
    }
    d[0] = 0;
    for (k = 1; k <= (size_t)M; k++) {
        size_t p = 3 * ((size_t)M - k);
        unsigned v = 0;
        int b;
        for (b = 2; b >= 0; b--) {
            size_t bit = p + (size_t)b;
            v = (v << 1) | ((q[bit / 32] >> (bit % 32)) & 1u);
        }
        d[k] = (unsigned char)v;
    }
    free(q); free(r); free(t);
    free(a004561_dig);
    a004561_dig = d;
    a004561_ndig = M;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > a004561_ndig) {
        idx_t M = n + n / 2 + 64;
        a004561_compute(M);
    }
    return (seq_t)a004561_dig[n];
}
