/*
 * A076969  a(1) = 1, a(n+1)= smallest cube greater than the n-th partial sum.
 * https://oeis.org/A076969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = smallest cube greater than the n-th partial
 *            sum
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   floor_cube_root, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_cube_root.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;
    static seq_t sum         = 0;

    /* produce terms up to n */
    while (cache_len < n)
    {
        /* grow the cache if needed */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* next term: smallest cube > sum */
        seq_t k = floor_cube_root(sum);
        k += (seq_t)1;                       /* cube root of the next cube */
        seq_t cube = k * k * k;              /* the cube itself */

        cache[cache_len] = cube;              /* store */
        sum += cube;                         /* update partial sum */
        ++cache_len;
    }

    return cache[n - 1];
}
