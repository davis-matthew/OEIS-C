/*
 * A126590  Multiples of 3 or 5 but not both.
 * https://oeis.org/A126590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126590: numbers k such that (k mod 3)*(k mod 5) = 0 and k mod 15 > 0.
   That is, multiples of 3 or 5 but not of 15.
   A(1)=3, A(2)=5, A(3)=6, A(4)=9, A(5)=10, A(6)=12, A(7)=18, ...
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms; grows on demand */
    static seq_t *cache = NULL;
    static size_t    cap   = 0;
    static size_t    len   = 0;
    static uint64_t  nexti = 1; /* next integer to test */

    if (n == 0)          /* defined only for n >= 1, but guard */
        return 0;

    /* Return cached value if already computed */
    if (n <= len)
        return cache[n - 1];

    /* initialise cache if not yet done */
    if (cap == 0) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we reach the requested index */
    while (len < n) {
        /* look for next integer satisfying the condition */
        while (!(((nexti % 3 == 0) || (nexti % 5 == 0)) && (nexti % 15 != 0))) {
            ++nexti;
        }
        /* grow cache if needed */
        if (len == cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        /* store the term */
        cache[len++] = (seq_t)nexti;
        ++nexti;
    }
    return cache[n - 1];
}
