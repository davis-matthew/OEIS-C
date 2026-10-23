/*
 * A022855  a(n) = [ a(n-1)/a(1) ] + [ a(n-1)/a(2) ] + ... + [ a(n-1)/a(n-1) ]
 *          for n >= 3, with initial terms 1,1.
 * https://oeis.org/A022855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} floor( a(n-1) / a(k) ), a(1)=1, a(2)=1.
 *            The function memoises the values so that any order of calls
 *            works
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache; cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated size */
    static size_t  cache_size = 0;  /* number of elements stored */

    if (n == (idx_t)0)
        return (seq_t)0;  /* undefined by OEIS offset; return 0 safely */

    /* Expand cache if necessary */
    if ((size_t)n > cache_size) {
        size_t new_cap = cache_cap;
        if (new_cap == 0)
            new_cap = 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;      /* double capacity */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate program (cannot recover). */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms, inclusive of n */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        if (i == 1 || i == 2) {
            cache[i - 1] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 2];  /* a(i-1) */
            seq_t sum = (seq_t)0;
            /* Sum floor(prev / a(k)) for k = 1 .. i-1  */
            for (size_t k = 1; k < i; ++k) {
                sum += prev / cache[k - 1];
            }
            cache[i - 1] = sum;
        }
    }
    cache_size = (size_t)n;
    return cache[n - 1];
}
