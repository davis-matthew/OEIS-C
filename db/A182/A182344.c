/*
 * A182344  Primes of the form n^4 + 3.
 * https://oeis.org/A182344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form m^4 + 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 23, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Cache of found primes of the form m^4 + 3. */
    static seq_t *cache   = NULL;   /* array of primes if found */
    static size_t  cap    = 0;      /* allocated capacity of the array */
    static size_t  len    = 0;      /* number of primes stored */
    static idx_t   next_m = 0;      /* next integer m to test */

    /* Generate primes until the cache contains at least n elements. */
    while (len < (size_t)n) {
        /* m^2 and m^4 are computed in type seq_t. */
        seq_t m_sq = (seq_t)next_m * next_m;   /* m^2  */
        seq_t val  = m_sq * m_sq + (seq_t)3;    /* m^4 + 3 */

        if (isPrimeGeneric(val)) {
            /* Grow the cache if necessary. */
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – terminate the program. */
                    abort();
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = val;
        }
        ++next_m;
    }

    return cache[n - 1];
}
