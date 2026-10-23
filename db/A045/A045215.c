/*
 * A045215  Numbers whose base-5 representation contains exactly three 0's and
 *          one 4.
 * https://oeis.org/A045215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly three 0's
 *            and exactly one 4. a(1) = 500, a(2) = 629, …
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Static cache of already computed terms.  All indices */
static seq_t *cache = NULL;    /* array of terms */
static size_t cache_size = 0;  /* number of terms stored */
static size_t cache_cap   = 0;  /* allocated capacity */

/* the largest integer examined so far; the next candidate is
   cache_next + 1.                                      */
static idx_t cache_next = 0;

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);               /* out of memory */
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* examine the next integer candidate */
        ++cache_next;

        idx_t x = cache_next;
        int zeros  = 0;
        int fours  = 0;

        /* count digits 0 and 4 in base‑5 representation */
        while (x > 0) {
            int d = (int)(x % 5);
            if (d == 0) ++zeros;
            else if (d == 4) ++fours;
            x /= 5;
        }

        /* accept numbers that contain exactly 3 zeros and 1 four */
        if (zeros == 3 && fours == 1) {
            /* grow the cache if necessary */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)cache_next;
        }
    }

    return cache[n - 1];
}
