/*
 * A031343  a(n) = prime(10*n).
 * https://oeis.org/A031343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(10*n)
 * Range:     u8 n <= 5, u16 n <= 654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9915 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static uint64_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static idx_t cached_limit = 0;

seq_t a(idx_t n)
{
    /* the index of the desired prime */
    idx_t target = (idx_t)10 * n;
    if (target == 0) return 0;  /* undefined for n = 0, but harmless */

    /* extend cache if needed */
    if ((size_t)target > cached_prime_count)
    {
        size_t new_limit = _prime_limit_for_idx(target);
        if ((idx_t)new_limit > cached_limit)
        {
            free(cached_primes);
            cached_primes = NULL;
            cached_prime_count = 0;
            cached_limit = 0;
            build_primes_upto(new_limit, &cached_primes, &cached_prime_count);
            cached_limit = (idx_t)new_limit;
        }
        /* In rare cases the estimate might be too low; retry until we have enough primes */
        while ((size_t)target > cached_prime_count) {
            new_limit = new_limit * 2 + 100;
            free(cached_primes);
            cached_primes = NULL;
            cached_prime_count = 0;
            build_primes_upto(new_limit, &cached_primes, &cached_prime_count);
            cached_limit = (idx_t)new_limit;
        }
    }

    /* primes are 1-indexed in the OEIS definition, array is 0-indexed */
    return (seq_t)cached_primes[target - 1];
}
