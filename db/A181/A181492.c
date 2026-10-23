/*
 * A181492  Primes of the form p=3*2^k+1 such that p-2 is also a prime.
 * https://oeis.org/A181492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form p = 3*2^k + 1 such that p-2 is also prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t p = 3 * pow2(next_k) + 1;
        if (is_prime_big(p)) {
            seq_t p_minus_2 = p - 2;
            if (is_prime_big(p_minus_2)) {
                /* Store the found prime in the cache. */
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = p;
            }
        }
        next_k++;
    }

    return cache[n - 1];
}
