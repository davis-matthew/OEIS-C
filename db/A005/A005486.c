/*
 * A005486  Decimal expansion of cube root of 6.
 * https://oeis.org/A005486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(6^(1/3) * 10^(n-1)) mod 10, computed with the
 *            digit-by-digit integer cube-root method
 * Range:     u8 n <= 9342, u16 n <= 9342, u32 n <= 9342, u64 n <= 9342,
 *            u128 n <= 9342, big any n
 * Helpers:   a005486_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a005486_le.h"

#define A005486_BASE 1000000000ULL

/* t = 300*S*d + 30*y*d^2 + d^3 */
static void a005486_tcalc(uint64_t *t, const uint64_t *S, const uint64_t *y,
                          uint64_t d, size_t L) {
    uint64_t c = d * d * d;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t v = S[i] * 300 * d + y[i] * 30 * d * d + c;
        t[i] = v % A005486_BASE;
        c = v / A005486_BASE;
    }
}

static unsigned char *a005486_compute(idx_t N) {
    size_t cap = (size_t)((2 * N + 20) / 9 + 6);
    uint64_t *y = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *S = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *R = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *t = (uint64_t *)calloc(cap, sizeof(uint64_t));
    unsigned char *dig = (unsigned char *)malloc((size_t)N + 1);
    idx_t k;
    for (k = 1; k <= N; k++) {
        size_t L = (size_t)((2 * k + 10) / 9 + 3);
        size_t i;
        uint64_t c, d, lo, hi;
        if (L > cap) L = cap;
        /* R = R*1000 + next group */
        c = (k == 1) ? 6 : 0;
        for (i = 0; i < L; i++) {
            uint64_t v = R[i] * 1000 + c;
            R[i] = v % A005486_BASE;
            c = v / A005486_BASE;
        }
        /* largest d with T(d) <= R */
        lo = 0; hi = 9;
        while (lo < hi) {
            uint64_t mid = (lo + hi + 1) / 2;
            a005486_tcalc(t, S, y, mid, L);
            if (a005486_le(t, R, L)) lo = mid; else hi = mid - 1;
        }
        d = lo;
        /* R -= T(d) */
        a005486_tcalc(t, S, y, d, L);
        c = 0;
        for (i = 0; i < L; i++) {
            uint64_t sub = t[i] + c;
            if (R[i] >= sub) { R[i] -= sub; c = 0; }
            else { R[i] = R[i] + A005486_BASE - sub; c = 1; }
        }
        /* S = 100*S + 20*y*d + d^2 (uses old y) */
        c = d * d;
        for (i = 0; i < L; i++) {
            uint64_t v = S[i] * 100 + y[i] * 20 * d + c;
            S[i] = v % A005486_BASE;
            c = v / A005486_BASE;
        }
        /* y = 10*y + d */
        c = d;
        for (i = 0; i < L; i++) {
            uint64_t v = y[i] * 10 + c;
            y[i] = v % A005486_BASE;
            c = v / A005486_BASE;
        }
        dig[k - 1] = (unsigned char)d;
    }
    free(y); free(S); free(R); free(t);
    return dig;
}

seq_t a(idx_t n) {
    static unsigned char *digits = NULL;
    static idx_t computed = 0;
    if (n > computed) {
        idx_t N = n + 16;
        if (N < 2 * computed) N = 2 * computed;
        if (digits) free(digits);
        digits = a005486_compute(N);
        computed = N;
    }
    return (seq_t)digits[n - 1];
}
