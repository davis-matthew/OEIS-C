/*
 * A026050  a(n) = (d(n)-r(n))/2, where d = A026049 and r is the periodic
 *          sequence with fundamental period (1,0,0,1).
 * https://oeis.org/A026050
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A026050
    a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) - a(n-6),
    for n >= 13, with initial values
    a(7)=59, a(8)=78, a(9)=102, a(10)=132, a(11)=170, a(12)=217.
    This recurrence follows from the generating‑function denominator
    (x^2+1)(x-1)^4.
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /*  Below the offset 7 the values are undefined – return 0. */
    if (n < 7)
        return 0;

    /*  Memoization cache: cache[i] holds a(i) for i >= 7.  */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /*  highest index already stored  */

    /*  Initialise cache with the known first six values.  */
    if (cache == NULL) {
        cap = 12;
        cache = (seq_t *)calloc(cap + 1, sizeof(seq_t));
        if (!cache) abort();
        cache[7] = 59;
        cache[8] = 78;
        cache[9] = 102;
        cache[10] = 132;
        cache[11] = 170;
        cache[12] = 217;
    }

    /*  Grow the cache if a larger index is requested.  */
    if ((size_t)n > cap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        if (newcap > cap) {
            /*  Zero the newly allocated part.  */
            size_t i;
            for (i = cap + 1; i <= newcap; ++i)
                tmp[i] = 0;
        }
        cache = tmp;

        /*  Compute terms from cap+1 up to n using the recurrence.  */
        idx_t i;
        for (i = cap + 1; i <= n; ++i) {
            seq_t v = 4 * cache[(size_t)i - 1]
                    - 7 * cache[(size_t)i - 2]
                    + 8 * cache[(size_t)i - 3]
                    - 7 * cache[(size_t)i - 4]
                    + 4 * cache[(size_t)i - 5]
                    -   cache[(size_t)i - 6];
            cache[(size_t)i] = v;
        }
        cap = newcap;
    }

    return cache[(size_t)n];
}
