/*
 * A206982  Number of n X 3 0..1 arrays avoiding the patterns 0 1 0 or 1 0 1
 *          in any row, column, diagonal or antidiagonal.
 * https://oeis.org/A206982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4) - 2*a(n-5) for n>7
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=6, a(2)=36, a(3)=58, a(4)=158, a(5)=420, a(6)=1066, a(7)=2754 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current allocated length (indices 0..cache_len-1) */

    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, but guard against nonsense calls */

    if (cache_len <= n)
    {
        size_t new_len = (size_t)n + 1;   /* array must hold indices up to n */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                  /* allocation failure */
        cache = tmp;

        /* Base values for n = 1..7 */
        static const seq_t base_vals[8] =
        {
            0ULL,            /* unused */
            (seq_t)6,
            (seq_t)36,
            (seq_t)58,
            (seq_t)158,
            (seq_t)420,
            (seq_t)1066,
            (seq_t)2754
        };

        size_t i;
        for (i = 1; i <= 7 && i <= n; ++i)
            if (cache_len <= i)
                cache[i] = base_vals[i];

        /* Compute recurrence for indices >= 8 */
        size_t start = (cache_len > 7) ? cache_len : 8;
        for (i = start; i <= n; ++i)
        {
            cache[i] =
                (seq_t)2 * cache[i-1] + cache[i-2] +
                (seq_t)2 * cache[i-3] - cache[i-4] -
                (seq_t)2 * cache[i-5];
        }

        cache_len = new_len;
    }

    return cache[n];
}
