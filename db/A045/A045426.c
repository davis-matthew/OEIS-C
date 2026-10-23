/*
 * A045426  Primes congruent to {1, 3, 4, 6} mod 7.
 * https://oeis.org/A045426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045426: primes p with p mod 7 ∈ {1,3,4,6} */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache   = NULL;   /* stored primes */
static idx_t   sz      = 0;     /* number of cached primes */
static size_t  cap     = 0;     /* allocated capacity */
static idx_t   next    = 2;     /* next integer to test */

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for offset 0 */
        return 0;

    /* extend cache until we have n primes */
    while (sz < n) {
        for (;; ++next) { /* find next prime satisfying the residue condition */
            if (is_prime(next) &&
                (next % 7ULL == 1U ||
                 next % 7ULL == 3U ||
                 next % 7ULL == 4U ||
                 next % 7ULL == 6U))
                break;
        }

        /* grow array if necessary */
        if (sz == cap) {
            size_t new_cap = cap ? cap << 1 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        cache[sz++] = (seq_t)next; /* store found prime */
        ++next;                    /* advance to next candidate */
    }

    return cache[n - 1];
}
