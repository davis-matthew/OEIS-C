/*
 * A126358  Number of base 4 n-digit numbers with adjacent digits differing by
 *          one or less.
 * https://oeis.org/A126358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=10; for n≥3: a(n)=3*a(n-1)-a(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached values */
    static size_t cache_len = 0;         /* number of cached entries */

    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;

        /* compute new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)4;
            else if (i == 2)     cache[i] = (seq_t)10;
            else                 cache[i] = 3 * cache[i - 1] - cache[i - 2];
        }
        cache_len = new_len;
    }
    return cache[n];
}
