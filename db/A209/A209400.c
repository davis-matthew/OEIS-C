/*
 * A209400  Number of subsets of {1,...,n} containing a subset of the form
 *          {k,k+1,k+3} for some k.
 * https://oeis.org/A209400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-4) - a(n-5) - 2*a(n-6)
 *            a(0)=a(1)=a(2)=a(3)=0, a(4)=2, a(5)=7
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises values so it works for arbitrary call order. */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* For n < 4 the value is 0 (pattern cannot appear). */
    if (n < 4)
        return 0;

    size_t want = (size_t)n + 1;

    if (cache_len < want) {
        /* grow the cache */
        seq_t *new_cache = (seq_t *)realloc(cache, want * sizeof(seq_t));
        if (!new_cache)                 /* allocation failure */
            abort();
        cache = new_cache;

        /* initialise the first 6 values if they are not already present */
        for (size_t i = cache_len; i < 6 && i < want; ++i) {
            switch (i) {
                case 0: case 1: case 2: case 3:
                    cache[i] = 0;      break;
                case 4: cache[i] = 2; break;
                case 5: cache[i] = 7; break;
            }
        }

        /* compute terms from the first missing index up to 'want' */
        size_t start = (cache_len > 6) ? cache_len : 6;

        for (size_t i = start; i < want; ++i) {
            seq_t res = 3 * cache[i - 1];
            res -= cache[i - 2];
            res -= 2 * cache[i - 3];
            res += cache[i - 4];
            res -= cache[i - 5];
            res -= 2 * cache[i - 6];
            cache[i] = res;
        }

        cache_len = want;
    }

    return cache[(size_t)n];
}
