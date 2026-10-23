/*
 * A142480  Primes congruent to 7 mod 51.
 * https://oeis.org/A142480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 7 (mod 51)
 * Range:     u8 n <= 3, u16 n <= 194, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;           /* cached primes */
static size_t cache_cap = 0;          /* current allocated cache size */
static idx_t primes_found = 0;        /* number of primes stored so far */
static idx_t current_candidate = 7;   /* next integer to test */

/* Return the nth prime congruent to 7 modulo 51 (OEIS A142480). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset safety */

    /* Expand cache until we have at least n terms */
    while (primes_found < n) {
        /* Check congruence first to reduce prime tests */
        if ((current_candidate % 51ULL) == 7ULL && is_prime(current_candidate)) {
            /* Ensure capacity for the new element */
            if (primes_found >= cache_cap) {
                /* Grow capacity to at least primes_found+1 */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                while (new_cap <= (size_t)primes_found) new_cap *= 2;
                ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
            }
            cache[primes_found++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }

    return cache[n - 1];
}
