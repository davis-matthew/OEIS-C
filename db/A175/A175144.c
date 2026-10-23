/*
 * A175144  a(n) = d(p(n)-1) + d(p(n)+1), where p(n) is the n-th prime, and
 *          where d(m) is the number of divisors of m.
 * https://oeis.org/A175144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(p(n)-1) + d(p(n)+1) where p(n) is the n‑th prime and
 *            d(m) is the number of positive divisors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/divisor_count_u64.h"

/* Compute the n-th term of sequence A175144.
   The function is thread‑safe apart from the static cache and is
   memoised so that repeated calls with any ordering of n remain fast. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of bounds */

    /* static cache of primes */
    static uint64_t *primes = NULL;   /* array of primes */
    static size_t prime_cnt = 0;      /* how many primes we have */
    static idx_t prime_limit = 0;     /* last limit used for build_primes_upto */

    /* A crude upper bound on the n‑th prime:
         For n ≤ 10000,  n * 20   > p(n)   (since 10000th prime ≈ 104729).   */
    idx_t needed_limit = n * 20 + 100;

    if (prime_limit < needed_limit || prime_cnt < (size_t)n) {
        /* Rebuild the prime list at a larger limit. */
        uint64_t *tmp = NULL;
        size_t   tmp_cnt = 0;

        build_primes_upto(needed_limit, &tmp, &tmp_cnt);

        /* Replace the old cache. */
        if (primes != NULL) free(primes);
        primes = tmp;
        prime_cnt = tmp_cnt;
        prime_limit = needed_limit;
    }

    /* Retrieve the n‑th prime (array is 0‑based). */
    uint64_t p = primes[n - 1];

    /* Compute the divisor counts of p−1 and p+1. */
    uint64_t d_minus = divisor_count_u64(p - 1);
    uint64_t d_plus  = divisor_count_u64(p + 1);

    /* Sum and cast to the output type. */
    return (seq_t)(d_minus + d_plus);
}
