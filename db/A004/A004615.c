/*
 * A004615  Divisible only by primes congruent to 1 mod 5.
 * https://oeis.org/A004615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2153, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   all_primes_1_mod_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th m >= 1 whose prime factors are all == 1 (mod 5); such m are == 1 (mod 10) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/all_primes_1_mod_5.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t lastm = 0;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t m = (lastm == 0) ? 1 : lastm + 10;
        while (!all_primes_1_mod_5(m)) m += 10;
        lastm = m;
        if (nterms == capterms) {
            idx_t nc = capterms ? capterms * 2 : 1024;
            terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
            capterms = nc;
        }
        terms[nterms++] = m;
    }
    return (seq_t)terms[n - 1];
}
