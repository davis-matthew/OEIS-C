/*
 * A142680  Primes congruent to 25 mod 57.
 * https://oeis.org/A142680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence : prim<sup>ed</sup> numbers congruent to 25 mod 57 (OEIS A142680) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_cache.h"

/* a(n) = the n-th prime p with p ≡ 25 (mod 57), indexed from n = 1 */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;          /* cached values */
    static size_t cache_size = 0;        /* number of cached elements */
    static size_t cache_cap  = 0;        /* capacity of cache array */
    static idx_t next_candidate = 139;   /* first candidate to test */

    /* if value already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* ensure cache capacity for at least n elements */
    if (n > cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* generate further primes congruent to 25 (mod 57) */
    idx_t cand = next_candidate;
    while (cache_size < n) {
        if (is_prime(cand))
            cache[cache_size++] = (seq_t)cand;
        cand += 57;
    }
    /* remember last tested candidate plus 57 for next round */
    next_candidate = cand;

    return cache[n - 1];
}
