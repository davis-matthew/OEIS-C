/*
 * A186594  Number of (n+3)X7 0..3 arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A186594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)-a(n-2)+3*a(n-4)-6*a(n-5)+3*a(n-6)+3*a(n-8)-3*a(n-9) for
 *            n > 14 with initial values a(1)=3306753, a(2)=104547,
 *            a(3)=407733, a(4)=2526165, a(5)=5080629, a(6)=5654805,
 *            a(7)=5711805, a(8)=5769381, a(9)=5831421, a(10)=5959701,
 *            a(11)=6475413, a(12)=9316941, a(13)=19268469, a(14)=36584472
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static const seq_t init[15] = {
    /* 0 unused */ 0,
    3306753, 104547, 407733, 2526165, 5080629, 5654805,
    5711805, 5769381, 5831421, 5959701, 6475413,
    9316941, 19268469, 36584472
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out of defined range */

    static seq_t *cache = NULL;                /* dynamic cache array */
    static size_t cap   = 0;                   /* current capacity (elements) */
    static idx_t  computed_max = 0;            /* highest index computed so far */

    /* Ensure cache has room for indices 0..n (we ignore index 0) */
    if (cache == NULL) {
        cache = seq_alloc_array((size_t)n + 1);
        cap   = (size_t)n + 1;
    } else if (cap < (size_t)n + 1) {
        seq_t *new_cache = seq_alloc_array((size_t)n + 1);
        for (idx_t i = 0; i <= computed_max; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cap   = (size_t)n + 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = init[i];
        } else {
            cache[i] = 2 * cache[i-1]
                     - cache[i-2]
                     + 3 * cache[i-4]
                     - 6 * cache[i-5]
                     + 3 * cache[i-6]
                     + 3 * cache[i-8]
                     - 3 * cache[i-9];
        }
    }
    if (n > computed_max) computed_max = n;

    return cache[n];
}
