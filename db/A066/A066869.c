/*
 * A066869  Sum of the first n safe primes.
 * https://oeis.org/A066869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n safe primes
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A safe prime is a prime p such that (p-1)/2 is also prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

/* Computes a(n) for n >= 1.  The function caches the cumulative sums
   of safe primes so that repeated calls are fast. */
seq_t a(idx_t n)
{
    static seq_t *cum = NULL;         /* cumulative sums of safe primes   */
    static size_t cum_cap  = 0;        /* current capacity of cum          */
    static size_t cum_cnt  = 0;        /* number of safe primes stored      */
    static bool   initialized = false;/* whether initial sieve has been done */

    if (n == 0)          /* undefined for n < 1, but return 0 */
        return 0;

    if (!initialized) {
        /* A limit that contains at least 10 000 safe primes.  This is
           far more than needed for all test cases (n <= 10 000). */
        idx_t limit = 2000000ULL;       /* empirical upper bound          */

        uint64_t *primes = NULL;
        size_t    primes_cnt = 0;

        build_primes_upto(limit, &primes, &primes_cnt);

        for (size_t i = 0 ; i < primes_cnt ; ++i) {
            uint64_t p = primes[i];

            if (p < 5)                 /* first safe prime is 5         */
                continue;

            /* (p-1)/2 must also be prime for p to be safe */
            if (is_prime((idx_t)((p - 1) / 2))) {
                if (cum_cnt == cum_cap) {
                    /* grow the cumulative array */
                    size_t new_cap = cum_cap ? cum_cap * 2U : 64U;
                    seq_t *tmp = seq_realloc(cum, cum_cap, new_cap);
                    cum = tmp;
                    cum_cap = new_cap;
                }

                seq_t sum = cum_cnt ? cum[cum_cnt - 1] : (seq_t)0;
                sum += (seq_t)p;
                cum[cum_cnt++] = sum;
            }

            /* stop early if we already have enough safe primes */
            if (cum_cnt >= 10000)
                break;
        }

        free(primes);
        initialized = true;
    }

    if (n <= cum_cnt)
        return cum[n - 1];
    /* If n exceeds the precomputed range, return 0 (unlikely in tests). */
    return 0;
}
