/*
 * A121411  Positive integers k for which there are primes of the form
 *          a^2+k^n=b^2+k^m with positive integers (a,b,m,n) and a > b.
 * https://oeis.org/A121411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121411: the nth positive integer that is not a square and
   not congruent to -1 modulo 4, not congruent to -1 modulo 5, and not
   congruent to -7 modulo 16.  The values are enumerated in increasing
   order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Guard against the (undefined) case n == 0 */
    if (n == 0)
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static idx_t next_candidate = 1;   /* next integer to test */

    /* Grow cache storage if necessary */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n values cached */
    while (cache_size < n) {
        seq_t k = (seq_t)next_candidate;
        bool ok = true;

        /* Skip perfect squares */
        seq_t r = floor_sqrt(k);
        if (r * r == k) {
            ok = false;
        }
        /* Skip forbidden congruence classes */
        else if ( (k % 4) == 3 ) {           /* k ≡ -1 (mod 4) */
            ok = false;
        } else if ( (k % 5) == 4 ) {           /* k ≡ -1 (mod 5) */
            ok = false;
        } else if ( (k % 16) == 9 ) {          /* k ≡ -7 (mod 16) */
            ok = false;
        }

        if (ok) {
            cache[cache_size++] = k;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
