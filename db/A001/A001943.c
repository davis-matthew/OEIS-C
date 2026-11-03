/*
 * A001943  Expansion of reciprocal of theta series of E_8 lattice.
 * https://oeis.org/A001943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -240 * Sum_{j=1..n} sigma_3(j) * a(n-j), a(0) = 1 (1/E_4
 *            with E_4 = 1 + 240*Sum_{k>=1} sigma_3(k) q^k)
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 8, i128 n <= 16,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 380 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *a001028_z(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *a001028_grow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = a001028_z(newlen);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t *a001943_R = 0;
static idx_t a001943_len = 0;

seq_t a(idx_t n) {
    if (n < a001943_len) return a001943_R[n];

    idx_t newlen = n + 1;
    a001943_R = a001028_grow(a001943_R, a001943_len, newlen);
    if (a001943_len == 0) {
        a001943_R[0] = 1;
        a001943_len = 1;
    }

    uint64_t *s3 = (uint64_t *)calloc((size_t)newlen, sizeof(uint64_t));
    for (idx_t d = 1; d < newlen; d++) {
        uint64_t c = (uint64_t)d * d * d;
        for (idx_t m = d; m < newlen; m += d) s3[m] += c;
    }

    for (idx_t k = a001943_len; k < newlen; k++) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= k; j++) {
            sum += a001943_R[k - j] * (seq_t)s3[j];
        }
        a001943_R[k] = sum * (-240);
    }
    a001943_len = newlen;
    free(s3);
    return a001943_R[n];
}
