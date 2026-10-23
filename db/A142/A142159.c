/*
 * A142159  Primes congruent to 1 mod 39.
 * https://oeis.org/A142159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 1 modulo 39. Index starts at 1. */
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined offset; return 0 as a safe value */

    static seq_t *cache = NULL;    /* Cache of found primes */
    static idx_t cache_len = 0;    /* Number of cached primes */
    static idx_t cache_cap = 0;    /* Current cache capacity */
    static idx_t next_k = 2;       /* k such that 39*k+1 = 79 is first term */

    /* Extend cache until it contains at least n elements */
    while (cache_len < n) {
        idx_t candidate = 39 * next_k + 1;  /* Next number of the form 39k+1 */

        if (is_prime(candidate)) {
            /* Ensure cache has space for one more element */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++next_k;
    }

    /* Return the (n‑1)th entry, because cache is 0‑based while a(1) is the first term */
    return cache[n - 1];
}
