/*
 * A031343  a(n) = prime(10*n).
 * https://oeis.org/A031343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(10*n)
 * Range:     u8 n <= 5, u16 n <= 654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

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
