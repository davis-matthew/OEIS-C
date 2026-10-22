/*
 * A004557  Expansion of sqrt(5) in base 4.
 * https://oeis.org/A004557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5) * 4^(n-1)) mod 4, computed by the exact
 *            digit-by-digit binary square root of 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004557_cmp, a004557_shl, a004557_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004557_cmp.h"
#include "../utils/a004557_shl.h"
#include "../utils/a004557_sub.h"

static unsigned char *a004557_dig = NULL;
static idx_t a004557_have = 0;

static void a004557_compute(idx_t N) {
    idx_t nb = 2 * N;
    size_t L = (size_t)(nb / 32 + 4);
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *bits = (unsigned char *)malloc((size_t)nb);
    idx_t i, k;

    for (i = 0; i < nb; i++) {
        /* radicand 5 = binary 01 01 followed by zero pairs */
        uint32_t pair = (i < 2) ? 1u : 0u;
        a004557_shl(r, r, L, 2);
        r[0] |= pair;
        a004557_shl(t, p, L, 2);
        t[0] |= 1u;
        if (a004557_cmp(r, t, L) >= 0) {
            a004557_sub(r, t, L);
            a004557_shl(p, p, L, 1);
            p[0] |= 1u;
            bits[i] = 1;
        } else {
            a004557_shl(p, p, L, 1);
            bits[i] = 0;
        }
    }

    free(a004557_dig);
    a004557_dig = (unsigned char *)malloc((size_t)N);
    for (k = 0; k < N; k++)
        a004557_dig[k] = (unsigned char)(bits[2 * k] * 2 + bits[2 * k + 1]);
    a004557_have = N;

    free(p);
    free(r);
    free(t);
    free(bits);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > a004557_have) {
        idx_t N = a004557_have * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a004557_compute(N);
    }
    return (seq_t)a004557_dig[n - 1];
}
