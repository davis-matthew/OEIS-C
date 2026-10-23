/*
 * A016085  a(1) = 1; a(n+1) = floor((sum{k=1 to n} a(k)^3)^(1/3)).
 * https://oeis.org/A016085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; a(n+1)=floor((∑_{k=1}^n a(k)^3)^(1/3))
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 38, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;      /* allocated capacity */
    static idx_t  size    = 0;      /* terms stored so far */
    static seq_t sum_cubes = 0;     /* ∑ a(k)^3 for k = 1..size */

    /* The sequence is defined for n ≥ 1 */
    if (n == 0)
        return 0;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;              /* allocation failed */

        /* a(1) = 1 */
        cache[0] = 1;
        sum_cubes = cache[0] * cache[0] * cache[0];
        size = 1;
    }

    /* Return cached value if already known */
    if (n <= size)
        return cache[n - 1];

    /* Generate terms up to the requested index */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t next = floor_cube_root(sum_cubes);  /* a(i) */

        /* Grow cache if needed */
        if (size >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;          /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        cache[size] = next;
        sum_cubes += next * next * next;
        ++size;
    }

    return cache[n - 1];
}
