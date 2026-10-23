/*
 * A166356  Expansion of e.g.f. 1 + x*arctanh(x), even powers only.
 * https://oeis.org/A166356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(1) = 2; a(n) = 2*n*(2*n-3)*a(n-1) for n >= 2
 *            (equivalently a(n) = (2n-1)! + (2n-2)! for n > 0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A166356. */
seq_t a(idx_t n)
{
    /* cache of already computed values */
    static seq_t *cache      = NULL;   /* array of cached terms      */
    static idx_t cache_len   = 0;      /* current number of entries  */

    /* enlarge cache if necessary */
    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure – return 0 */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)2;
            else
            {
                seq_t prev = cache[i - 1];
                seq_t prod = (seq_t)(2 * i) * (seq_t)(2 * i - 3);
                cache[i] = prev * prod;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
