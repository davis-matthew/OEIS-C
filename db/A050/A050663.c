/*
 * A050663  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 1.
 * https://oeis.org/A050663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p such that A048853(p) = 1; computed by
 *            incremental search over primes, counting primes reachable by
 *            changing one digit
 * Range:     u8 none, u16 n <= 2, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static int A050663_count_gt1(uint64_t p)
{
    uint8_t digs[20];
    int d = 0;
    uint64_t t = p;
    while (t > 0) { digs[d++] = (uint8_t)(t % 10); t /= 10; }
    /* digs[0] is least significant */
    int count = 0;
    uint64_t place = 1;
    for (int pos = 0; pos < d; ++pos) {
        int orig = digs[pos];
        uint64_t base = p - (uint64_t)orig * place;
        for (int nd = 0; nd <= 9; ++nd) {
            if (nd == orig) continue;
            if (pos == d - 1 && nd == 0 && d > 1) continue;
            uint64_t cand = base + (uint64_t)nd * place;
            if (cand < 2) continue;
            if (is_prime_u64(cand)) {
                if (++count > 1) return count;
            }
        }
        place *= 10;
    }
    return count;
}

static uint64_t *A050663_terms = 0;
static size_t A050663_cnt = 0, A050663_cap = 0;
static uint64_t A050663_next = 2;

seq_t a(idx_t n)
{
    while (A050663_cnt < (size_t)n) {
        uint64_t p = A050663_next;
        A050663_next = (p == 2) ? 3 : p + 2;
        if (!is_prime_u64(p)) continue;
        if (A050663_count_gt1(p) != 1) continue;
        if (A050663_cnt == A050663_cap) {
            A050663_cap = A050663_cap ? A050663_cap * 2 : 1024;
            A050663_terms = (uint64_t *)realloc(A050663_terms, A050663_cap * sizeof(uint64_t));
        }
        A050663_terms[A050663_cnt++] = p;
    }
    return (seq_t)A050663_terms[(size_t)n - 1];
}
