/*
 * A065878  Numbers which are not an integer multiple of their number of
 *          binary 1's.
 * https://oeis.org/A065878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A065878:  
   a(n) is the n‑th positive integer that is NOT an integer multiple of
   its binary popcount (number of 1‑bits).  The function is
   memoised and works for any of the supported seq_t types. */

#include <stdlib.h>

/* count 1-bits of a seq_t value */
static unsigned int popcount_seq_t(seq_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;   /* delete lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms (index 0 unused) */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;   /* number of filled entries, 0 ≤ len ≤ cap */

    if (cache == NULL) {
        cap  = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;    /* dummy */
        len = 1;
    }

    if ((size_t)n < len)
        return cache[(size_t)n];

    seq_t cand = cache[len - 1] + 1;

    /* extend cache until we have computed a(n) */
    while (len <= (size_t)n) {
        if (cand == 0)            /* guard against wrap‑around overflow */
            break;

        unsigned int pc = popcount_seq_t(cand);
        if (pc && cand % (seq_t)pc != 0) {
            if (len >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = cand;
        }
        ++cand;
    }

    return cache[(size_t)n];
}
