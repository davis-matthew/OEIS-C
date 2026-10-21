/*
 * A212900  Number of (w,x,y,z) with all terms in {0,...,n} and distinct
 *          consecutive gap sizes.
 * https://oeis.org/A212900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (A212900):
   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) + a(n-5)
          + 3*a(n-6) - a(n-7) - 2*a(n-8) + a(n-9)
   Initial values:
   a(0)=0, a(1)=4, a(2)=28, a(3)=122, a(4)=340,
   a(5)=786, a(6)=1558, a(7)=2814, a(8)=4690, a(9)=7404
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static size_t last_computed = 9; /* highest index whose value is known */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 10;                      /* need indices 0..9 inclusive */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory: return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)28;
        cache[3] = (seq_t)122;
        cache[4] = (seq_t)340;
        cache[5] = (seq_t)786;
        cache[6] = (seq_t)1558;
        cache[7] = (seq_t)2814;
        cache[8] = (seq_t)4690;
        cache[9] = (seq_t)7404;
        last_computed = 9;
    }

    /* if the requested value has already been computed, return it */
    if ((size_t)n <= last_computed)
        return cache[(size_t)n];

    /* ensure the cache can hold values up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms from last_computed+1 up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val = (seq_t)2 * cache[i - 1];
        val += cache[i - 2];
        val -= (seq_t)3 * cache[i - 3];
        val -= cache[i - 4];
        val += cache[i - 5];
        val += (seq_t)3 * cache[i - 6];
        val -= cache[i - 7];
        val -= (seq_t)2 * cache[i - 8];
        val += cache[i - 9];
        cache[i] = val;
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}
