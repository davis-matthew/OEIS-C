/*
 * A106941  Primes of the form 4x^2-3xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p that can be written as 4x^2 - 3xy + 5y^2
 *            * with non‑negative integers x and y, indexed from 1
 * Range:     u8 n <= 6, u16 n <= 566, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9847 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106941: primes of the form 4x^2 - 3xy + 5y^2 with x, y >= 0. All values of the
 * (positive definite) form up to a bound L are enumerated in 64 bits, the
 * primes kept, sorted and deduplicated; L doubles until there are n terms.
 * For fixed y the form is smallest at x = max(0, -B y/(2A)), and at least
 * (4AC - B^2) y^2 / (4A) (C y^2 if B >= 0), which bounds y. (The original
 * worked in seq_t, too slow for big.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

#define QA (4)
#define QB (-3)
#define QC (5)

static uint64_t *terms = NULL;
static size_t term_len = 0;

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static void build(int64_t L)
{
    size_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof *t);
    if (!t) abort();
    for (int64_t y = 0;; ++y) {
        /* the least value of the form at this y, over real x >= 0 */
        double lo = QB >= 0 ? (double)QC * y * y : (4.0 * QA * QC - (double)QB * QB) * y * y / (4.0 * QA);
        if (lo > (double)L) break;
        int64_t x0 = QB >= 0 ? 0 : (int64_t)((double)(-QB) * y / (2.0 * QA));
        /* walk down from x0 and up from x0 + 1 while the value is <= L */
        for (int dir = -1; dir <= 1; dir += 2)
            for (int64_t x = dir < 0 ? x0 : x0 + 1; x >= 0; x += dir) {
                int64_t v = QA * x * x + QB * x * y + QC * y * y;
                if (v > L) break;
                if (v >= 2 && is_prime_u64((uint64_t)v)) {
                    if (cnt == cap) {
                        uint64_t *u = (uint64_t *)realloc(t, (cap *= 2) * sizeof *t);
                        if (!u) abort();
                        t = u;
                    }
                    t[cnt++] = (uint64_t)v;
                }
            }
    }
    qsort(t, cnt, sizeof *t, cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (!u || t[i] != t[u - 1]) t[u++] = t[i];
    free(terms);
    terms = t;
    term_len = u;
}

seq_t a(idx_t n)
{
    static int64_t L = 0;
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        L = L ? 2 * L : 1 << 16;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
