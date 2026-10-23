/*
 * A039383  Numbers whose base-9 representation has the same nonzero number of
 *          4's and 5's.
 * https://oeis.org/A039383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th positive integer whose base‑9 representation
 * contains the same non‑zero number of digits “4” and “5”.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously found terms */
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;
    static size_t cache_cap = 0;
    static seq_t next_check = 0;   /* last value tested + 1 */

    /* grow the cache until we have at least n terms */
    while (cache_sz < n) {
        /* find next qualifying number */
        do {
            ++next_check;
            seq_t tmp = next_check;
            int c4 = 0, c5 = 0;
            while (tmp > (seq_t)0) {
                seq_t d = tmp % (seq_t)9;
                tmp /= (seq_t)9;
                if (d == (seq_t)4) ++c4;
                else if (d == (seq_t)5) ++c5;
            }
            if (c4 == c5 && c4 > 0) {
                /* ensure space in cache */
                if (cache_sz + 1 > cache_cap) {
                    if (cache_cap == 0) cache_cap = 16;
                    else cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_sz++] = next_check;
                break;   /* one term stored, go to outer loop */
            }
        } while (1);
    }

    return cache[(size_t)(n - 1)];
}
