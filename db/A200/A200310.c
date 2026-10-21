/*
 * A200310  a(n) = n-1 for n <= 4, otherwise if n is even then a(n) =
 *          a(n-5)+2^(n/2), and if n is odd then a(n) = a(n-1)+2^((n-3)/2).
 * https://oeis.org/A200310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A200310
   a(n) = n-1 for n <= 4
   if n is even then a(n) = a(n-5) + 2^(n/2)
   if n is odd  then a(n) = a(n-1) + 2^((n-3)/2)   (n >= 5)
   Offset: 1                                                */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* memoised terms, indices 0..cap-1 */
    static size_t cap = 0;               /* allocated capacity                */
    static size_t filled = 0;            /* highest index already computed    */

    if (n == 0) return 0;  /* undefined for n=0, return 0 for safety */

    /* Ensure capacity for index n */
    if ((idx_t)n >= (idx_t)cap) {
        size_t newcap = (cap == 0) ? 16 : cap * 2;
        while ((idx_t)newcap <= (idx_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    while ((idx_t)filled < (idx_t)n) {
        ++filled;
        if (filled <= 4) {
            cache[filled] = (seq_t)(filled - 1);            /* 0,1,2,3 */
        } else if ((filled & 1) == 0) {                     /* even  */
            cache[filled] = cache[filled - 5] + pow2(filled / 2);
        } else {                                           /* odd   */
            cache[filled] = cache[filled - 1] + pow2((filled - 3) / 2);
        }
    }
    return cache[n];
}
