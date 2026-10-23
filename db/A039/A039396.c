/*
 * A039396  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 4's.
 * https://oeis.org/A039396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039396: numbers whose base-10 representation has the same
   nonzero number of 0's and 4's.  Search implementation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;
    static size_t   cache_cap = 0;
    static idx_t    current   = 0;   /* last integer tested */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* fill cache until it contains at least n terms */
    while (cache_len < n) {
        /* find next qualifying integer */
        while (1) {
            current++;
            unsigned zeros = 0, fours = 0;
            idx_t tmp = current;
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 10);
                if (d == 0)   zeros++;
                else if (d == 4) fours++;
                tmp /= 10;
            }
            if (zeros == fours && zeros > 0)
                break;
        }

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)current;
    }

    return cache[n - 1];
}
