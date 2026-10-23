/*
 * A039422  Numbers whose base-10 representation has the same nonzero number
 *          of 3's and 9's.
 * https://oeis.org/A039422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A039422:
   Numbers whose decimal representation has the same nonzero
   number of 3's and 9's.
   The function searches numbers in increasing order, stores found
   values in a static cache, and returns the requested element. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* Static cache of previously found terms */
static seq_t *cache = NULL;      /* array of found values */
static size_t cache_len = 0;     /* number of values stored   */
static size_t cache_cap = 0;     /* current capacity          */
static seq_t last = 0;           /* last integer examined     */

seq_t a(idx_t n) {
    /* Continue searching until we have at least n terms in the cache */
    while (cache_len < (size_t)n) {
        /* Move to next integer */
        ++last;
        seq_t value = last;

        /* Count occurrences of digits 3 and 9 */
        unsigned c3 = 0, c9 = 0;
        seq_t tmp = value;
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            if (d == 3) ++c3;
            else if (d == 9) ++c9;
            tmp /= 10;
        }

        /* Store only if the counts match and are non‑zero */
        if (c3 == c9 && c3 > 0) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) { /* allocation failure — abort */
                    /* In a production system you would handle this more gracefully */
                    abort();
                }
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = value;
        }
    }
    /* OEIS indexing starts at 1 */
    return cache[(size_t)n - 1];
}
