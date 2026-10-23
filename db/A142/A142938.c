/*
 * A142938  Primes congruent to 29 mod 64.
 * https://oeis.org/A142938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime that is congruent to 29 modulo 64.
   The sequence is 1‑based: a(1) = 29.  The function uses a static cache
   so that calls in any order remain efficient. */
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    /* Undefined for n == 0 in OEIS convention */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found primes */
    static seq_t *cache   = NULL;   /* array of primes */
    static size_t cap     = 0;      /* allocated capacity of cache */
    static idx_t  count   = 0;      /* number of primes stored */
    static idx_t  next    = 29;     /* next candidate of the form 29 mod 64 */

    /* Extend the cache until the n‑th prime is available */
    while (count < n) {
        if (is_prime(next)) {
            /* Ensure there is room for another element. */
            if (cap == 0 || (size_t)count == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) /* allocation failed – abort */
                    abort();
                cache = tmp;
                cap   = newcap;
            }
            cache[count++] = (seq_t)next;
        }
        next += 64;
    }

    return cache[n - 1];
}
