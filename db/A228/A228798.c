/*
 * A228798  Number of 3 X n binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2) + 6*a(n-3) - a(n-4) - a(n-5) initial
 *            values: a(1)=4, a(2)=5, a(3)=36, a(4)=97, a(5)=411
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined below the offset */

    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* highest computed index */
    static size_t cache_cap = 0; /* allocated size (>= cache_len+1) */

    size_t required = (size_t)n + 1; /* we keep index 0 unused for simplicity */
    if (required > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure is unlikely */
        cache = tmp;
        cache_cap = required;
    }

    /* base values */
    static const seq_t init[5] = {
        (seq_t)4, (seq_t)5, (seq_t)36, (seq_t)97, (seq_t)411
    };

    /* compute missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 5) {
            cache[i] = init[i - 1];
        } else {
            seq_t val = cache[i - 1];
            val += (seq_t)8 * cache[i - 2];
            val += (seq_t)6 * cache[i - 3];
            val -= cache[i - 4];
            val -= cache[i - 5];
            cache[i] = val;
        }
    }
    cache_len = (size_t)n;
    return cache[n];
}
