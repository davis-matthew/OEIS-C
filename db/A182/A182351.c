/*
 * A182351  Primes of the form k^4 - 6.
 * https://oeis.org/A182351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form k^4 - 6 (k ≥ 1, offset 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *cache   = NULL;
    static idx_t  count   = 0;    /* number of primes stored */
    static idx_t  last    = 1;    /* last k examined (start from 1) */
    static size_t capacity = 0;   /* current cache capacity */

    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Find primes until we have at least n of them */
    while (count < n) {
        /* next k */
        last++;

        seq_t k  = (seq_t)last;
        seq_t k2 = k * k;         /* k^2 */
        seq_t k4 = k2 * k2;       /* k^4 */
        seq_t val = k4 - (seq_t)6;/* k^4 - 6 */

        if (isPrimeGeneric(val)) {
            /* grow cache if necessary */
            if (count == capacity) {
                capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[count++] = val;
        }
    }

    return cache[n - 1];
}
