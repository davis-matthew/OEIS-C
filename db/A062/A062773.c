/*
 * A062773  Index of the smallest prime which follows square of n-th prime.
 * https://oeis.org/A062773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi( nextprime( prime(n)^2 ) )
 * Range:     u8 n <= 6, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 981 (2026-11-01)
 * Helpers:   build_primes_upto, next_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"
#include "../utils/prime_pi.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    /* Offset starts at 1; undefined for n==0 */
    if (n == 0) return 0;

    /* static cache of primes up to the largest n requested */
    static idx_t cached_max_n = 0;
    static uint64_t *prime_cache = NULL;
    static size_t prime_cache_cnt = 0;

    /* Rebuild cache only when a larger n is requested */
    if (n > cached_max_n) {
        idx_t limit = _prime_limit_for_idx(n);
        /* Ensure the sieve produces at least n primes */
        while (1) {
            if (prime_cache != NULL) {
                free(prime_cache);
                prime_cache = NULL;
            }
            build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
            if (prime_cache_cnt >= n) break;
            limit *= 2;  /* increase bound and retry */
        }
        cached_max_n = n;
    }

    /* nth prime (0‑based index) */
    uint64_t p_n = prime_cache[n - 1];

    /* Square of the prime, cast to seq_t to avoid overflow in seq_t context */
    seq_t sq = (seq_t)p_n * p_n;
    uint64_t sq64 = (uint64_t)sq;

    /* Smallest prime strictly greater than p_n^2 */
    uint64_t q = next_prime(sq64);

    /* pi(q) is the index of q */
    return prime_pi(q);
}
