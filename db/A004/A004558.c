/*
 * A004558  Expansion of sqrt(5) in base 5.
 * https://oeis.org/A004558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5) * 5^(n-1)) mod 5; digits are produced by
 *            the base-5 digit-by-digit square root algorithm (bignum,
 *            memoized)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq5_cmp, sq5_muladd, sq5_mulsmall, sq5_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sq5_cmp.h"
#include "../utils/sq5_muladd.h"
#include "../utils/sq5_mulsmall.h"
#include "../utils/sq5_sub.h"

static unsigned char *sq5_dig = NULL;
static idx_t sq5_ndig = 0;

static void sq5_compute(idx_t N) {
    size_t L = (size_t)(N * 7 / 96) + 6;
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *dig = (unsigned char *)malloc((size_t)N);
    idx_t k;
    r[0] = 5; /* leading digit pair "10" in base 5 */
    for (k = 0; k < N; k++) {
        uint32_t d = 4;
        for (;;) {
            if (d == 0) { memset(q, 0, L * sizeof(uint32_t)); break; }
            sq5_mulsmall(q, y, L, d);
            if (sq5_cmp(q, r, L) <= 0) break;
            d--;
        }
        sq5_sub(r, q, L);
        dig[k] = (unsigned char)d;
        sq5_muladd(y, L, 5, 10 * d);
        sq5_muladd(r, L, 25, 0);
    }
    free(r); free(y); free(q);
    free(sq5_dig);
    sq5_dig = dig;
    sq5_ndig = N;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > sq5_ndig) {
        idx_t N = sq5_ndig * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        sq5_compute(N);
    }
    return (seq_t)sq5_dig[n - 1];
}
