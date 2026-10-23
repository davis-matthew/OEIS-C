/*
 * A142912  Primes congruent to 41 mod 63.
 * https://oeis.org/A142912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 183, u32 n <= 129, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime congruent to 41 (mod 63).  a(1) = 41. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset is irrelevant */

    /* static cache of already found primes */
    static seq_t *cache = NULL;            /* dynamic array of terms */
    static idx_t  count = 0;               /* number of terms stored          */
    static idx_t  cap   = 0;               /* allocated capacity              */
    static seq_t  next  = (seq_t)41;       /* next candidate of the form 63*k+41 */

    if (count == 0)                       /* initialise lazily */
        next = (seq_t)41;

    /* grow cache if needed */
    if (count < n) {
        if (cap < n) {
            idx_t new_cap = cap ? (cap << 1) : 8;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache,
                                           (size_t)new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* generate missing terms */
        while (count < n) {
            while (!is_prime_big(next))    /* keep searching until a prime */
                next = next + (seq_t)63;

            cache[count++] = next;         /* store the prime */
            next = next + (seq_t)63;       /* next candidate of the form 63*k+41 */
        }
    }

    return cache[n - 1];                  /* sequence is 1‑based */
}
