/*
 * A234154  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having the absolute values of all six edge and diagonal
 *          differences no larger than 1.
 * https://oeis.org/A234154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) - 6*a(n-3) with a(1) = 46, a(2) =
 *            226, a(3) = 1108
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Static cache to store computed values for all n >= 1 */
    static seq_t *cache = NULL; /* array indexed by 0..n-1 */
    static idx_t stored = 0;    /* number of values stored (cache[0] = a(1)) */

    /* Extend cache if necessary */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        stored = 0;
    }
    else if (n > stored)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
    }

    /* Compute values up to n */
    for (idx_t i = stored; i < n; ++i)
    {
        seq_t val;
        idx_t idx = i + 1; /* current n value */

        if (idx == 1)
            val = (seq_t)46;
        else if (idx == 2)
            val = (seq_t)226;
        else if (idx == 3)
            val = (seq_t)1108;
        else
        {
            /* recurrence: a(n) = 7*a(n-1) - 9*a(n-2) - 6*a(n-3) */
            seq_t a1 = cache[idx - 2];  /* a(n-1) */
            seq_t a2 = cache[idx - 3];  /* a(n-2) */
            seq_t a3 = cache[idx - 4];  /* a(n-3) */

            val = 7 * a1;
            val -= 9 * a2;
            val -= 6 * a3;
        }

        cache[i] = val;
    }

    stored = n;
    return cache[n - 1];
}
