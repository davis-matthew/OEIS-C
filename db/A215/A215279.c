/*
 * A215279  Primes congruent to {2, 3, 4} mod 11.
 * https://oeis.org/A215279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   compute primes congruent to 2, 3 or 4 modulo 11 by simple
 *            searching
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of the sequence and the current candidate */
    static seq_t *cache = NULL;     /* array of values returned so far   */
    static size_t cap    = 0;       /* allocated capacity of cache       */
    static idx_t  count  = 0;       /* number of elements stored         */
    static idx_t  cur    = 2;       /* next integer to examine           */

    /* a(1) is the first term, behavior for n <= 0 is irrelevant */
    if (n <= 0)
        return (seq_t)0;

    /* Grow the cache until it holds n elements */
    while (count < n) {
        /* Ensure enough space in the cache */
        if (count == cap) {
            size_t newcap = cap ? cap * 2 : 128;
            void *tmp = realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)    /* allocation failed – give up */
                return (seq_t)0;
            cache = (seq_t *)tmp;  /* cast for C++ compatibility */
            cap   = newcap;
        }

        /* Test the current candidate for primality */
        if (is_prime(cur)) {
            idx_t r = cur % 11;
            if (r == 2 || r == 3 || r == 4) {
                cache[count++] = (seq_t)cur;
            }
        }
        ++cur;
    }

    return cache[n - 1];
}
