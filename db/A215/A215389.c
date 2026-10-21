/*
 * A215389  Primes congruent to {2, 3, 6} mod 19.
 * https://oeis.org/A215389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 881, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes congruent to 2, 3, or 6 modulo 19.
   The sequence is the list of all primes p such that
   p mod 19 ∈ {2, 3, 6}.  a(1)=2, a(2)=3, a(3)=41, …   */
/* The implementation uses a cache that grows on demand.  Prime testing
   is performed with the library function is_prime_big, which accepts
   a value of type seq_t.  The cache is stored as an array of seq_t
   and is resized with the helper ensure_capacity, which works for both
   trivial and arbitrary‑precision seq_t types. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* provides is_prime_big(seq_t) */
#include "../utils/ensure_capacity.h" /* provides ensure_capacity */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* current capacity of cache */
    static idx_t last_checked = 1;/* integer last examined during search */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Search for further primes until we have at least n terms */
    for (idx_t cand = last_checked + 1; ; ++cand) {
        idx_t r = cand % 19;
        if (r == 2 || r == 3 || r == 6) {
            if (is_prime_big((seq_t)cand)) {
                /* Grow cache if necessary */
                size_t new_len = cache_len + 1;
                ensure_capacity(new_len, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
                cache[cache_len++] = (seq_t)cand;
                last_checked = cand;
                if (n <= cache_len)
                    return cache[n - 1];
            }
        }
    }
}
