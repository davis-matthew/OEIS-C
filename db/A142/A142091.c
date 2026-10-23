/*
 * A142091  Primes congruent to 23 mod 35.
 * https://oeis.org/A142091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 217, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth prime that is congruent to 23 (mod 35).  The value is
   obtained by incremental search and memoisation on the cache of
   previously found terms.  The sequence has no closed‐form formula. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based: a(1) = 23, a(2) = 163, … */
    if (n < 1)
        return 0;                    /* undefined for n < 1 */

    static seq_t *cache = NULL;      /* cached primes */
    static size_t cap   = 0;          /* current allocation size   */
    static idx_t  len   = 0;          /* number of primes stored    */

    /* Return cached value if already computed */
    if (n <= len)
        return cache[n - 1];

    /* Initialise cache with the first term 23 if it is empty */
    if (len == 0) {
        ensure_capacity(1, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)23;
        len = 1;
    }

    /* Search for further primes until the nth one is found */
    seq_t last = cache[len - 1];
    while (len < n) {
        seq_t candidate = last + (seq_t)35;   /* next number congruent to 23 mod 35 */

        /* Test primality */
        if (is_prime_big(candidate)) {
            ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[len] = candidate;
            ++len;
        }
        last = candidate;
    }

    return cache[n - 1];
}
