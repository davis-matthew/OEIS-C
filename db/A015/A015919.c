/*
 * A015919  Positive integers k such that 2^k == 2 (mod k).
 * https://oeis.org/A015919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 2^k == 2 (mod k)
 * Range:     u8 n <= 55, u16 n <= 6607, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9162 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A015919: positive k with 2^k == 2 (mod k), tested in order with exact 64-bit
 * modular exponentiation. Even k are tested too: 161038 is the first even
 * term (the original skipped them, and lost it and later even terms). */

#include <stdint.h>
#include <stdlib.h>

static uint64_t pow2_mod(uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m, b = 2 % m;
    for (; e; e >>= 1) {
        if (e & 1) r = (uint64_t)((unsigned __int128)r * b % m);
        b = (uint64_t)((unsigned __int128)b * b % m);
    }
    return r;
}

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        uint64_t k = next_k++;
        if (pow2_mod(k, k) != 2 % k) continue;
        if (term_len == term_cap) {
            term_cap = term_cap ? 2 * term_cap : 1024;
            uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
            if (!t) abort();
            terms = t;
        }
        terms[term_len++] = k;
    }
    return (seq_t)terms[n - 1];
}
