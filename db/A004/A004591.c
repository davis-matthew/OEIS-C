/*
 * A004591  Expansion of sqrt(10) in base 8.
 * https://oeis.org/A004591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(sqrt(10) * 8^(n-1)) mod 8; digits are computed with
 *            the digit-by-digit (long-division style) square root in base 8,
 *            using multiword integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004591_cmp, a004591_mulsmall, a004591_shl, a004591_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004591_cmp.h"
#include "../utils/a004591_mulsmall.h"
#include "../utils/a004591_shl.h"
#include "../utils/a004591_sub.h"

static unsigned char *a004591_dig = NULL;
static idx_t a004591_ndig = 0;

static void a004591_compute(idx_t N) {
    size_t L = (size_t)((3 * N + 64) / 32 + 2);
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *v = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *dig = (unsigned char *)malloc((size_t)N);
    idx_t i;
    for (i = 0; i < N; i++) {
        uint32_t d;
        /* bring down next base-8 digit pair: 10 = "12"_8, then "00" */
        a004591_shl(r, L, 6);
        if (i == 0) r[0] |= 10u;
        memcpy(q, p, L * sizeof(uint32_t));
        a004591_shl(q, L, 4); /* q = 16*p */
        for (d = 7; d > 0; d--) {
            a004591_mulsmall(v, q, L, d);
            if (a004591_cmp(v, r, L) <= 0) break;
        }
        if (d > 0) a004591_sub(r, v, L);
        a004591_shl(p, L, 3);
        p[0] |= d;
        dig[i] = (unsigned char)d;
    }
    free(r); free(p); free(q); free(v);
    free(a004591_dig);
    a004591_dig = dig;
    a004591_ndig = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a004591_ndig) {
        idx_t N = 2 * a004591_ndig;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a004591_compute(N);
    }
    return (seq_t)a004591_dig[n - 1];
}
