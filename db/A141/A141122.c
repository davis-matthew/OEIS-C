/*
 * A141122  Duplicate of A068228.
 * https://oeis.org/A141122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the nth prime of the form 12k+1
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but callers are expected to use n >= 1. */
    if (n == 0) return 0;                 /* safety guard */

    static seq_t *cache = NULL;           /* cached primes of the form 12k+1 */
    static size_t cap   = 0;              /* allocated capacity of cache     */
    static size_t sz    = 0;              /* number of primes stored so far  */
    static seq_t  next  = 13;             /* next candidate of the form 12k+1*/
    static int    init  = 0;              /* flag whether initialization done */

    if (!init) {
        init = 1;
        cap  = 16;                        /* initial allocation */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* If the requested index is already cached, return it. */
    if (n <= sz) return cache[n - 1];

    /* Generate primes until we have at least n of them. */
    while (sz < n) {
        if (is_prime_small(next)) {
            if (sz == cap) {              /* grow the cache */
                cap *= 2;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = next;
        }
        next += 12;                       /* next number of the form 12k+1 */
    }

    return cache[n - 1];
}
