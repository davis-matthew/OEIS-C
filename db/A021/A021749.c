/*
 * A021749  Decimal expansion of 1/745.
 * https://oeis.org/A021749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/745 via long division.
   a(0) = 0  (integer part)
   For n >= 1, a(n) = digit at position n+1 in the decimal expansion. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of digits: cache[0] = integer part (0),
       cache[1] = first decimal digit, cache[2] = second, etc. */
    static seq_t *cache   = NULL;   /* array of digits */
    static size_t cap    = 0;       /* allocated capacity (elements) */
    static idx_t  last   = 0;       /* highest index stored in cache */
    static uint64_t rem  = 1;       /* remainder before next digit */

    /* First call: initialise cache with integer part */
    if (cache == NULL) {
        cap    = 1;
        cache  = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = 0;          /* integer part of 1/745 */
        last    = 0;
    }

    /* Need digits up to index n+1 (so cache must contain n+2 elements) */
    if ((size_t)n + 2 > cap) {
        size_t new_cap = (size_t)n + 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute missing digits up to index n+1 (inclusive) */
    while (last < n + 1) {
        rem *= 10ULL;
        seq_t digit = (seq_t)(rem / 745ULL);
        rem       = rem % 745ULL;
        cache[++last] = digit;
    }

    /* Return the required term */
    if (n == 0)
        return cache[0];
    else
        return cache[n + 1];
}
