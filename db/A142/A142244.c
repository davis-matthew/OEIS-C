/*
 * A142244  Primes congruent to 23 mod 42.
 * https://oeis.org/A142244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p % 42 == 23 (OEIS A142244)
 * Range:     u8 n <= 5, u16 n <= 552, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static memoization */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_sz  = 0;
    static idx_t   next_k    = 0;  /* k index for the next candidate: p = 42*k + 23 */

    if (n == 0) return 0;          /* valid indices start at 1 */

    while (cache_sz < n) {
        idx_t candidate = 42 * next_k + 23;
        if (is_prime(candidate)) {
            /* grow storage if needed */
            ensure_capacity(cache_sz + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_sz++] = (seq_t)candidate;
        }
        next_k++;
    }

    return cache[n - 1];
}
