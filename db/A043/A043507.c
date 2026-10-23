/*
 * A043507  Numbers having three 4's in base 10.
 * https://oeis.org/A043507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth natural number whose decimal representation
 *            contains exactly three digits equal to 4, listed in increasing
 *            order
 * Range:     u8 none, u16 n <= 698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 we return 0 as a
       harmless placeholder; the value will never be used. */
    if (n == 0) return 0;

    /* Static cache state – shared among all calls to a(). */
    static seq_t *cache = NULL;   /* array of terms so far */
    static size_t cap   = 0;      /* current capacity of cache */
    static size_t sz    = 0;      /* number of terms stored */
    static idx_t cur    = 1;      /* next integer to test for inclusion */

    /* Generate terms until we have at least n of them. */
    while (sz < (size_t)n) {
        idx_t tmp   = cur;
        unsigned count = 0;

        /* Count the number of digit '4' in cur. */
        while (tmp > 0) {
            if ((tmp % 10) == 4) {
                ++count;
                if (count > 3) break;  /* early exit if more than 3 */
            }
            tmp /= 10;
        }
        if (count == 3) {
            /* Ensure the cache has room for a new entry. */
            if (sz == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *newbuf = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newbuf) abort();   /* allocation failure – terminate */
                cache = newbuf;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)cur;   /* store the term (possible truncation) */
        }
        ++cur;  /* move to the next integer */
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
