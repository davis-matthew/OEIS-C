/*
 * A142740  Primes congruent to 13 mod 59.
 * https://oeis.org/A142740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 107, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Sequentially search for primes congruent to 13 modulo 59. The function caches
   results so that repeated calls for arbitrary indices are efficient. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t* cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 13;   /* the first number to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset < 1, but return 0 */
    /* Ensure we have at least n primes computed. */
    while (cache_len < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            /* Grow cache if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
            cache[cache_len++] = next_candidate;
        }
        /* Advance to the next number that is 13 mod 59. */
        next_candidate += 59;
    }
    return cache[n - 1];
}
