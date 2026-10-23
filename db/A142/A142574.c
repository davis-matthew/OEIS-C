/*
 * A142574  Primes congruent to 44 mod 53.
 * https://oeis.org/A142574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 53 == 44
 * Range:     u8 n <= 1, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap   = 0;
    static size_t cache_size  = 0;
    static uint64_t next_start = 1;   /* next number to search from (exclusive) */

    while (cache_size < n) {
        uint64_t prime;

        /* find the next prime that satisfies p % 53 == 44 */
        do {
            prime = next_prime(next_start);   /* next prime > next_start */
            next_start = prime + 1;           /* start searching after this prime */
        } while ((prime % 53ULL) != 44ULL);

        /* grow cache if necessary */
        ensure_capacity(cache_size + 1, &cache_cap,
                        (void **)&cache, sizeof(seq_t));

        cache[cache_size++] = (seq_t)prime;
    }

    return cache[n - 1];
}
