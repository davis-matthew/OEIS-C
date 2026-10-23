/*
 * A142666  Primes congruent to 2 mod 57.
 * https://oeis.org/A142666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime that is congruent to 2 modulo 57
 * Range:     u8 n <= 3, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by an incremental search: after the first prime 2,
   we test numbers of the form 2 + 57*k and keep those that are prime. */

#include <stdlib.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) returns true if the argument is prime */

seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1, but guard against 0 */
        return (seq_t)0;

    /* static cache of found primes */
    static seq_t *cache       = NULL;
    static size_t  cap        = 0;   /* allocated capacity */
    static size_t  size       = 0;   /* number of stored primes */
    static uint64_t last_seen = 2;   /* last number processed (initially the first prime) */

    /* initialise on first call */
    if (cache == NULL) {
        cap  = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();              /* out of memory */
        cache[0] = (seq_t)2;                     /* first prime in the progression */
        size     = 1;
        last_seen = 2;
    }

    /* generate further primes until we have at least n entries */
    while (size < n) {
        last_seen += 57;                          /* next candidate congruent to 2 mod 57 */
        if (is_prime((idx_t)last_seen)) {
            if (size >= cap) {                   /* grow cache */
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[size++] = (seq_t)last_seen;     /* store prime */
        }
    }

    return cache[n - 1];
}
