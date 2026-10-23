/*
 * A140143  a(1)=1, a(n)=a(n-1)+n^0 if n odd, a(n)=a(n-1)+ n^5 if n is even.
 * https://oeis.org/A140143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=a(n-1)+1 if n odd, a(n)=a(n-1)+n^5 if n
 *            even
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* highest index computed (>=1) */

    if (n == 0)
        return 0;

    if (cache == NULL)
    {
        cache = (seq_t*)calloc(2, sizeof(seq_t));   /* indices 0 and 1 */
        cache[1] = 1;
        cache_size = 1;
    }

    if ((size_t)n > cache_size)
    {
        size_t new_size = (size_t)n + 1;           /* indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                              /* allocation failed */
        cache = tmp;

        for (idx_t i = (idx_t)(cache_size + 1); i <= n; ++i)
        {
            seq_t add;
            if (i & 1)
                add = 1;
            else
            {
                seq_t v = (seq_t)i;
                add = v * v * v * v * v;             /* i^5 */
            }
            cache[(size_t)i] = cache[(size_t)(i - 1)] + add;
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n];
}
