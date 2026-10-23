/*
 * A142014  Primes congruent to 10 mod 31.
 * https://oeis.org/A142014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 10 (mod 31). Search-based
 *            implementation
 * Range:     u8 n <= 3, u16 n <= 215, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* Cached list of found primes */
static idx_t  cache_len = 0;     /* Number of primes stored */
static size_t cache_cap = 0;     /* Current capacity of the cache array */
static uint64_t next_k = 1;      /* Next coefficient for numbers 31*k+10 */

/*
 * Returns the n-th prime p such that p ≡ 10 (mod 31).
 * The function is memoised and works for any call order.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; n ≥ 1 is expected */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate initial cache if needed */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n of them */
    while (cache_len < n) {
        /* candidate numbers of the form 31*k + 10, starting at k = 1 => 41 */
        seq_t candidate = (seq_t)31 * (seq_t)next_k + (seq_t)10;
        next_k++;  /* advance for next iteration */

        if (is_prime_big(candidate)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;  /* double capacity */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];
}
