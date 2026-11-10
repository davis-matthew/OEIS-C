/*
 * A050664  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 2.
 * https://oeis.org/A050664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p with A048853(p) = 2 (primes obtainable by
 *            changing one digit of p, same digit count); found by incremental
 *            search over primes
 * Range:     u8 none, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static int A050664_count(uint64_t p)
{
    uint8_t digs[20];
    int d = 0;
    uint64_t t = p;
    while (t) { digs[d++] = (uint8_t)(t % 10); t /= 10; }
    /* digs[0] is least significant */
    int count = 0;
    uint64_t place = 1;
    for (int pos = 0; pos < d; ++pos) {
        int orig = digs[pos];
        uint64_t base = p - (uint64_t)orig * place;
        for (int nd = 0; nd <= 9; ++nd) {
            if (nd == orig) continue;
            if (pos == d - 1 && nd == 0) continue;
            uint64_t cand = base + (uint64_t)nd * place;
            if (is_prime_u64(cand)) {
                ++count;
                if (count > 2) return count;
            }
        }
        place *= 10;
    }
    return count;
}

static uint64_t *A050664_terms = 0;
static size_t A050664_n = 0, A050664_cap = 0;
static uint64_t A050664_next = 2;

seq_t a(idx_t n)
{
    while (A050664_n < (size_t)n) {
        uint64_t p = A050664_next;
        A050664_next = (p == 2) ? 3 : p + 2;
        if (!is_prime_u64(p)) continue;
        if (A050664_count(p) != 2) continue;
        if (A050664_n == A050664_cap) {
            size_t nc = A050664_cap ? A050664_cap * 2 : 256;
            A050664_terms = (uint64_t *)realloc(A050664_terms, nc * sizeof(uint64_t));
            A050664_cap = nc;
        }
        A050664_terms[A050664_n++] = p;
    }
    return (seq_t)A050664_terms[(size_t)n - 1];
}
