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
 * Helpers:   build_primes_upto, next_prime, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

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

        seq_t part = seq_pow((seq_t)p, exp);
        prod = seq_mul(prod, part);

        last_start = p + 1;                 /* next interval starts after this prime */
    }

    free(primes);
    return prod;
}
