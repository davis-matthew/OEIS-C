/*
 * A005365  Hoggatt sequence with parameter d=7.
 * https://oeis.org/A005365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), T(n,k) = Product_{j=0..6} C(n+j,k) /
 *            Product_{m=1..6} C(k+m,m) (the 7F6(...;-1) series term by term);
 *            T(n,0) = 1, T(n,k+1) = T(n,k) * Product_{j=0..6} (n-k+j) /
 *            Product_{m=1..7} (k+m). Fixed-width builds evaluate T(n,k) from
 *            its prime factorization (Legendre) so no int...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 421 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static uint64_t leg_A005365(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

static seq_t a_fixed_A005365(idx_t n) {
    uint64_t lim = (uint64_t)n + 6;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim + 1));
    uint64_t *base = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim + 1));
    size_t np = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            pr[np++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    for (size_t t = 0; t < np; t++) {
        uint64_t p = pr[t], e = 0;
        for (uint64_t j = 0; j <= 6; j++) e += leg_A005365((uint64_t)n + j, p);
        for (uint64_t m = 1; m <= 6; m++) e += leg_A005365(m, p);
        base[t] = e;
    }
    seq_t sum = 0;
    for (uint64_t k = 0; k <= (uint64_t)n; k++) {
        seq_t term = 1;
        for (size_t t = 0; t < np; t++) {
            uint64_t p = pr[t];
            uint64_t e = base[t] - leg_A005365(k, p);
            for (uint64_t j = 0; j <= 6; j++) e -= leg_A005365((uint64_t)n - k + j, p);
            for (uint64_t m = 1; m <= 6; m++) e -= leg_A005365(k + m, p);
            if (e) {
                seq_t b = (seq_t)p, r = 1;
                while (e) {
                    if (e & 1) r *= b;
                    e >>= 1;
                    if (e) b *= b;
                }
                term *= r;
            }
        }
        sum += term;
    }
    free(comp);
    free(pr);
    free(base);
    return sum;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed_A005365(n);
    seq_t t = 1, sum = 1;
    for (idx_t k = 0; k < n; k++) {
        seq_t num = 1, den = 1;
        for (idx_t j = 0; j <= 6; j++) num *= (seq_t)(n - k + j);
        for (idx_t m = 1; m <= 7; m++) den *= (seq_t)(k + m);
        t = t * num / den;
        sum += t;
    }
    return sum;
}
