/*
 * A039045  Numbers whose base-7 representation has the same number of 3's and
 *          5's.
 * https://oeis.org/A039045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039045: numbers whose base-7 representation has the same
   number of 3's and 5's.  Computed by scanning natural numbers and
   selecting those with equal counts of 3 and 5. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t cache_cap = 0;   /* current capacity of cache */
static idx_t cache_size = 0;  /* number of cached values */
static seq_t next_val = 0;    /* next integer to test */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used – offset is 1 */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* extend the cache until we have at least n entries */
    while (cache_size < n) {
        seq_t val = next_val;

        /* count digits 3 and 5 in base 7 */
        seq_t tmp = val;
        int c3 = 0, c5 = 0;
        if (tmp == 0) {
            /* counts remain zero – qualifies automatically */
        } else {
            while (tmp > 0) {
                int d = (int)(tmp % 7);
                if (d == 3) ++c3;
                else if (d == 5) ++c5;
                tmp /= 7;
            }
        }

        if (c3 == c5) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(EXIT_FAILURE);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }

        /* advance to next integer to test */
        next_val = val + 1;
    }

    return cache[n - 1];
}
