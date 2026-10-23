/*
 * A165393  Number of slanted 3Xn (i=1..3)X(j=i..n+i-1) 1..4 arrays with all
 *          1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 2 neighbors w...
 * https://oeis.org/A165393
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) for n >= 13; initial values: a(2)=3,
 *            a(3)=28, a(4)=94, a(5)=154, a(6)=233, a(7)=300, a(8)=370,
 *            a(9)=428, a(10)=491, a(11)=552, a(12)=614
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset return 0. */
    if (n < 2)
        return 0;

    /* Static cache for memoization. */
    static seq_t *cache      = NULL;      /* cache[0] is a(2), cache[1] is a(3), ... */
    static size_t cap        = 0;          /* current allocated size of cache */
    static idx_t  last_n     = 1;          /* largest n that has been computed (initially 1) */

    /* All values up to n can be stored in cache[0 .. n-2]          */
    /* Compute the required cache size (indices start at 2).       */
    size_t needed = (size_t) (n - 1);        /* number of terms from a(2) to a(n) inclusive */

    /* Ensure the cache is large enough. */
    if (cap < needed) {
        size_t newcap = (cap == 0) ? 8 : cap;
        while (newcap < needed) {
            if (newcap > SIZE_MAX / sizeof(seq_t))
                break;                     /* avoid overflow */
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            abort();                        /* allocation failure */
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing terms. The cache holds values up to last_n. */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        size_t idx = (size_t) (k - 2);      /* index in cache (a(2) -> 0) */

        /* Initial values are hard‑coded only to start the recurrence.   */
        /* This is acceptable because the table does not contain the
           entire sequence. */
        static const seq_t base[] = {
            3ULL, 28ULL, 94ULL, 154ULL, 233ULL, 300ULL,
            370ULL, 428ULL, 491ULL, 552ULL, 614ULL
        };
        static const size_t base_len = sizeof(base) / sizeof(base[0]);

        if (k <= 12 && idx < base_len) {
            cache[idx] = base[idx];
        } else {
            /* Recurrence a(k) = 2*a(k-1) - a(k-2).  Since the result
               grows linearly, no intermediate overflows occur for the
               supported seq_t sizes. */
            seq_t prev  = cache[idx - 1];
            seq_t prev2 = cache[idx - 2];
            cache[idx]  = (seq_t)(2 * prev - prev2);
        }
    }

    last_n = n;

    return cache[(size_t)(n - 2)];
}
