/*
 * A008347  a(n) = Sum_{i=0..n-1} (-1)^i * prime(n-i).
 * https://oeis.org/A008347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 9968, u32 n <= 9968, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   get_prime, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008347
   Definition:  a(0) = 0,
                 a(n) = prime(n) - a(n-1)  (n ≥ 1)
   This implements the recurrence using the provided `get_prime` and
   `grow_seq` helper functions and a static cache for O(1) repeated
   evaluations.  The cache grows as needed. */
#include <stddef.h>          /* for size_t */
#include "../utils/get_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t */
    static size_t  cap    = 0;      /* current allocation size   */
    static size_t  len    = 0;      /* number of entries computed */

    /* If the requested index has already been computed, return it */
    if (n < len) return cache[(size_t)n];

    /* Ensure the array has sufficient capacity for indices up to n */
    if (cap < (size_t)n + 1) grow_seq(&cache, &cap, (size_t)n + 1);

    /* Initialise the first term if necessary */
    if (len == 0) {
        cache[0] = 0;
        len = 1;
    }

    /* Compute missing terms up to n using the recurrence a(i) = p(i) - a(i-1) */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t p = (seq_t)get_prime((idx_t)i);  /* p(i) = i-th prime */
        cache[i] = p - cache[i - 1];
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
