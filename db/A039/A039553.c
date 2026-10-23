/*
 * A039553  Numbers whose base-12 representation has the same nonzero number
 *          of 8's and 9's.
 * https://oeis.org/A039553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute A039553: Numbers whose base‑12 representation has the same
 * non‑zero number of 8’s and 9’s.  The function searches integers
 * sequentially, counts the 8’s and 9’s in base‑12, and caches the
 * qualifying numbers so that subsequent calls are O(1) for
 * already‑found positions.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n == 0. */
    if (n == 0) {
        return 0;
    }

    /* Static cache of the sequence values found so far. */
    static seq_t *cache      = NULL;   /* array of found numbers */
    static size_t cache_size = 0;     /* number of elements stored     */
    static size_t cache_cap  = 0;     /* allocated capacity            */
    static seq_t current = 1;         /* next integer to test          */

    /* Extend the cache until it contains at least n terms. */
    while ((size_t)cache_size < n) {
        /* Count 8's and 9's in the base‑12 representation of current. */
        seq_t tmp = current;
        unsigned int cnt8 = 0, cnt9 = 0;
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 12);
            if (d == 8) ++cnt8;
            else if (d == 9) ++cnt9;
            tmp /= 12;
        }

        if (cnt8 == cnt9 && cnt8 > 0) {      /* a valid term */
            if (cache_size == cache_cap) {   /* grow the cache if needed */
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                    new_cap * sizeof(seq_t));
                if (!new_cache) abort();    /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = current;    /* store the value */
        }

        ++current;      /* move to the next integer */
    }

    return cache[n - 1];
}
