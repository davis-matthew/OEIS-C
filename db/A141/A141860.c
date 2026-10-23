/*
 * A141860  Primes congruent to 2 mod 15.
 * https://oeis.org/A141860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 610, u32 n <= 800, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime p such that p % 15 == 2   (offset 1) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes congruent to 2 mod 15. */
    static seq_t *cache   = NULL;     /* array of values   */
    static size_t cap     = 0;        /* allocated capacity */
    static size_t len     = 0;        /* number of stored primes */
    static seq_t last     = 0;        /* last prime stored */

    if (n == 0)                    /* undefined for offset 1 */
        return 0;

    /* Allocate initial cache if necessary. */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)         /* allocation failure */
            return 0;
    }

    /* Generate primes until we have at least n entries. */
    while (len < n) {
        seq_t candidate = (len == 0) ? (seq_t)2 : last + (seq_t)15;

        /* Find the next prime congruent to 2 mod 15. */
        while (!is_prime_big(candidate))
            candidate += (seq_t)15;

        /* Grow cache if needed. */
        if (len == cap) {
            cap <<= 1;                      /* double the capacity */
            seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (new_cache == NULL)          /* allocation failure */
                return 0;
            cache = new_cache;
        }

        cache[len++] = candidate;
        last = candidate;
    }

    return cache[n - 1];
}
