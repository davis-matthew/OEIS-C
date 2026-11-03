/*
 * A029743  Primes with distinct digits.
 * https://oeis.org/A029743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = the n-th prime whose decimal digits are all distinct
 * Range:     u8 n <= 41, u16 n <= 2272, u32 n <= 283086, u64 n <= 283086,
 *            u128 n <= 283086, big any n
 * Validated: full, pass, 283086 terms through n = 283086 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029743: primes with distinct digits (finite: 283086 terms, the largest
 * 987654103; a 10-digit number with distinct digits is divisible by 3). The
 * numbers with distinct digits are enumerated in increasing order, length by
 * length, by a depth-first search over digits, and tested with the exact
 * 64-bit primality test; the list is built once and cached (the original
 * tested every integer in turn and ran out of time). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint32_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;

static void dfs(uint64_t v, unsigned used, int left)
{
    if (!left) {
        if (is_prime_u64(v)) {
            if (term_len == term_cap) {
                term_cap = term_cap ? 2 * term_cap : 1 << 16;
                uint32_t *t = (uint32_t *)realloc(terms, term_cap * sizeof *t);
                if (!t) abort();
                terms = t;
            }
            terms[term_len++] = (uint32_t)v;
        }
        return;
    }
    for (unsigned d = (v == 0 ? 1 : 0); d < 10; ++d)
        if (!(used & (1u << d))) dfs(v * 10 + d, used | (1u << d), left - 1);
}

seq_t a(idx_t n)
{
    if (!terms)
        for (int len = 1; len <= 9; ++len) dfs(0, 0, len);
    if (n < 1 || (size_t)n > term_len) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
