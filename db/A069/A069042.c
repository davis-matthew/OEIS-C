/*
 * A069042  Numbers k such that A000364(k) == 1 (mod k^2).
 * https://oeis.org/A069042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k >= 1 with A000364(k) == 1 (mod k^2), where
 *            A000364(k) = A000111(2k) (Seidel-Entringer triangle computed mod
 *            k^2); found by search over k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static uint64_t *a69042_terms = NULL;
static size_t a69042_cnt = 0, a69042_cap = 0;
static uint64_t a69042_next = 1;
static uint64_t *a69042_p = NULL, *a69042_q = NULL;
static size_t a69042_bufsz = 0;

static int a69042_test(uint64_t k) {
    uint64_t mod = k * k;
    size_t M = (size_t)(2 * k);
    if (a69042_bufsz < M + 2) {
        free(a69042_p);
        free(a69042_q);
        a69042_bufsz = (M + 2) * 2;
        a69042_p = (uint64_t *)malloc(a69042_bufsz * sizeof(uint64_t));
        a69042_q = (uint64_t *)malloc(a69042_bufsz * sizeof(uint64_t));
    }
    uint64_t *p = a69042_p, *q = a69042_q;
    p[0] = 1 % mod;
    for (size_t m = 1; m <= M; m++) {
        q[0] = 0;
        uint64_t prev = 0;
        for (size_t j = 1; j <= m; j++) {
            uint64_t x = prev + p[m - j];
            if (x >= mod) x -= mod;
            q[j] = x;
            prev = x;
        }
        uint64_t *t = p; p = q; q = t;
    }
    return p[M] == 1 % mod;
}

seq_t a(idx_t n) {
    while (a69042_cnt < (size_t)n) {
        uint64_t k = a69042_next++;
        if (a69042_test(k)) {
            if (a69042_cnt == a69042_cap) {
                a69042_cap = a69042_cap ? a69042_cap * 2 : 32;
                a69042_terms = (uint64_t *)realloc(a69042_terms, a69042_cap * sizeof(uint64_t));
            }
            a69042_terms[a69042_cnt++] = k;
        }
    }
    return (seq_t)a69042_terms[n - 1];
}
