/*
 * A015540  a(n) = 5*a(n-1) + 6*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A015540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Compute the n-th term of A015540.  */
seq_t a(idx_t n)
{
    /* static cache with separate length and capacity */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;   /* number of terms that are valid in cache   */
    static idx_t cache_cap = 0;   /* allocated capacity (number of elements)   */

    /* first call: allocate room for two initial terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache_len = 2;                     /* we will store terms 0 and 1          */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        assert(cache != NULL);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* if the requested index is beyond what has been computed            */
    if (n + 1 > cache_len) {

        /* extend the allocated capacity if necessary (doubling strategy) */
        if (n + 1 > cache_cap) {
            idx_t new_cap = cache_cap;
            /* avoid overflow in shifting */
            while (new_cap < n + 1) {
                if (new_cap == 0u) new_cap = 1;
                new_cap <<= 1;
            }
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            assert(tmp != NULL);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms from the current length up to n          */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = ((seq_t)5 * cache[i - 1]) + ((seq_t)6 * cache[i - 2]);
        }
        cache_len = n + 1;          /* now terms 0 .. n are valid              */
    }

    return cache[n];
}
