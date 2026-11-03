/*
 * A162457  Numbers whose prime factors when sorted and stacked fill an
 *          equilateral triangle.
 * https://oeis.org/A162457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth integer whose prime factors are distinct and their *
 *            decimal digit counts form the set {1,2,…,m} for some m
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   factor_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static uint64_t next_candidate = 2;

static bool is_factor_triangle(uint64_t x)
{
    if (x < 2) return false;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(x, pr, ex, &cnt);
    if (cnt == 0) return false;

    size_t m = cnt;
    bool used[64] = {0};
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] != 1) return false;
        uint64_t len = num_digits0(pr[i]);   /* number of decimal digits */
        if (len < 1 || len > m) return false;
        if (used[(size_t)len]) return false;
        used[(size_t)len] = true;
    }
    for (size_t l = 1; l <= m; ++l) {
        if (!used[l]) return false;
    }
    return true;
}

static void ensure_terms(idx_t n)
{
    if (terms_len >= n) return;

    if (terms_cap == 0) {
        terms_cap = 64;
        terms = mem_new(seq_t, terms_cap);
    }

    while (terms_len < n) {
        while (true) {
            if (next_candidate == UINT64_MAX) {
                /* Should never happen for requested n */
                return;
            }
            if (is_factor_triangle(next_candidate)) {
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    terms = mem_resize(seq_t, terms, terms_cap);
                }
                terms[terms_len++] = (seq_t)next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0 */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
