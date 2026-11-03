/*
 * A058943  Coefficients of irreducible polynomials over GF(2) listed in
 *          lexicographic order.
 * https://oeis.org/A058943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 127, u64 n <= 1377,
 *            u128 n <= 1377, big any n
 * Validated: standard, pass, 43 terms through n = 1335 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A058943: coefficients of irreducible monic polynomials
   over GF(2) listed in lexicographic order.  Each term is the decimal
   interpretation of the binary coefficient string. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* degree of a 32‑bit polynomial (position of highest 1 bit), or -1 if zero */
static inline int degree_uint32(uint32_t p)
{
    int d = -1;
    while (p) {
        p >>= 1;
        d++;
    }
    return d;
}

/* GF(2) polynomial reduction of p modulo mod (mod is monic) */
static inline uint32_t poly_mod_uint32(uint32_t p, uint32_t mod)
{
    int deg_mod = degree_uint32(mod);
    if (deg_mod < 0) return p;
    while (degree_uint32(p) >= deg_mod) {
        unsigned shift = degree_uint32(p) - deg_mod;
        p ^= mod << shift;
    }
    return p;
}

/* GF(2) polynomial multiplication (no modulus), operands fit in 32 bits */
static inline uint32_t poly_mul_uint32(uint32_t a, uint32_t b)
{
    uint32_t res = 0;
    while (b) {
        if (b & 1u) res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return res;
}

/* Convert binary polynomial to decimal integer represented in seq_t */
static seq_t poly_to_decimal_uint32(uint32_t poly, int deg)
{
    seq_t val = 0;
    for (int i = deg; i >= 0; --i) {
        unsigned bit = (poly >> i) & 1u;
        val = val * 10 + bit;
    }
    return val;
}

/* List of all monic irreducible polynomials found so far (by degree). */
static uint32_t *prev_poly = NULL;
static size_t prev_cnt = 0;
static size_t prev_cap = 0;

static void add_previous(uint32_t p)
{
    if (prev_cnt == prev_cap) {
        prev_cap = prev_cap ? prev_cap * 2 : 16;
        prev_poly = (uint32_t *)realloc(prev_poly, prev_cap * sizeof(uint32_t));
    }
    prev_poly[prev_cnt++] = p;
}

/* Test whether a monic polynomial poly of degree deg is irreducible over GF(2). */
static bool is_irreducible_uint32(uint32_t poly, int deg)
{
    if (deg == 1) return true;                 /* linear monic polynomials are irreducible */

    int half = deg / 2;
    for (size_t i = 0; i < prev_cnt; ++i) {
        int d = degree_uint32(prev_poly[i]);
        if (d > half) break;                   /* later factors must be > half */
        if (poly_mod_uint32(poly, prev_poly[i]) == 0)
            return false;                      /* divisible by a smaller irreducible */
    }
    return true;
}

/* Cache of sequence values produced so far. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap = new_cap;
}

/* Generate the next irreducible polynomial and store its decimal value. */
static void generate_next(seq_t *out)
{
    static int current_deg = 1;
    static uint32_t current_mask = 0;

    for (;;) {
        uint32_t max_mask = 1u << current_deg;   /* free lower bits count */
        while (current_mask < max_mask) {
            uint32_t poly = (1u << current_deg) | current_mask;
            ++current_mask;
            if (is_irreducible_uint32(poly, current_deg)) {
                *out = poly_to_decimal_uint32(poly, current_deg);
                add_previous(poly);            /* record for future checks */
                return;
            }
        }
        ++current_deg;
        current_mask = 0;
    }
}

/* Public interface: returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined below offset */

    size_t target = (size_t)n;
    ensure_capacity(target);
    while (seq_len < target) {
        generate_next(&seq_cache[seq_len]);
        ++seq_len;
    }
    return seq_cache[n - 1];
}
