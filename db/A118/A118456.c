/*
 * A118456  a(n) = Product_{k=1..n} P(k), where P(k) is the smallest prime >=
 *          k.
 * https://oeis.org/A118456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} P(k) where P(k) is the smallest prime ≥ k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 437 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_idx, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"
#include "../utils/next_prime.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Computes a(n) as defined in OEIS A118456 */
seq_t a(idx_t n)
{
    /* Value is undefined for n < 1; return neutral element for safety */
    if (n < 1) return seq_one();

    /* Determine the largest prime needed: the smallest prime ≥ n */
    uint64_t limit = next_prime((uint64_t)n);

    /* Build the list of all primes up to 'limit' */
    size_t prime_count;
    uint64_t *primes;
    build_primes_upto(limit, &primes, &prime_count);

    seq_t prod = seq_one();
    idx_t last_start = 1;            /* first k in current interval   */

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = (idx_t)primes[i];
        if (last_start > n) break;          /* finished all k */

        idx_t end = (n < p) ? n : p;        /* last k that maps to this prime */
        idx_t exp = end - last_start + 1;   /* exponent of prime p in the product */

        seq_t part = ipow_idx((seq_t)p, exp);
        prod = seq_mul(prod, part);

        last_start = p + 1;                 /* next interval starts after this prime */
    }

    free(primes);
    return prod;
}
