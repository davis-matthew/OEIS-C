/*
 * A008039  Coordination sequence T2 for Zeolite Code ATS.
 * https://oeis.org/A008039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008039:
   Recurrence: a(n) = a(n-3) + 2*a(n-5) - 2*a(n-8) - a(n-10) + a(n-13)  (n ≥ 14)
   Initial values:
       a(0)=1, a(1)=4, a(2)=10, a(3)=20, a(4)=32, a(5)=49,
       a(6)=73, a(7)=97, a(8)=124, a(9)=157, a(10)=193,
       a(11)=236, a(12)=281, a(13)=325
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[14] = {
        1ULL, 4ULL, 10ULL, 20ULL, 32ULL, 49ULL,
        73ULL, 97ULL, 124ULL, 157ULL, 193ULL,
        236ULL, 281ULL, 325ULL
    };

    /* Direct return for the first 14 terms */
    if (n < 14)
        return init[n];

    static seq_t *cache = NULL;          /* memoized values */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t  computed = 0;          /* largest index computed */

    /* Allocate/resize cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 14; ++i)
            cache[i] = init[i];
        computed = 13;
    } else if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute any missing terms */
    if (computed < n) {
        idx_t start = (computed < 13) ? 14 : computed + 1;
        for (idx_t k = start; k <= n; ++k) {
            seq_t left  = cache[k-3] + cache[k-5] + cache[k-5] + cache[k-13];
            seq_t right = cache[k-8] + cache[k-8] + cache[k-10];
            cache[k] = left - right;          /* left ≥ right for all n ≥ 14 */
        }
        computed = n;
    }

    return cache[n];
}
