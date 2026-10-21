/*
 * A231502  a(n) = Sum_{i=0..n} wt(i)^4, where wt() = A000120().
 * https://oeis.org/A231502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} wt(i)^4 where wt(i) is the binary popcount
 *            of i
 * Range:     u8 n <= 11, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;     /* cache of cumulative sums a(i) */
static idx_t a_cache_size = 0;    /* number of cached entries (a_cache[i] == a(i)) */

/* Compute a(n) with memoization */
seq_t a(idx_t n)
{
    /* Ensure cache covers indices 0..n */
    if (n >= a_cache_size)
    {
        /* Resize cache */
        seq_t *new_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * (n + 1));
        if (!new_cache)
            return 0; /* fallback on allocation failure */
        a_cache   = new_cache;

        /* Start filling from the first missing index */
        idx_t start   = a_cache_size;            /* next index to compute */
        seq_t cur_sum = (a_cache_size > 0) ? a_cache[a_cache_size - 1] : 0;

        for (idx_t i = start; i <= n; ++i)
        {
            /* Compute popcount of i via the Kernighan method */
            idx_t x   = i;
            unsigned int cnt = 0;
            while (x)
            {
                ++cnt;
                x &= x - 1;
            }

            /* w = cnt, compute w^4 */
            seq_t w   = (seq_t)cnt;
            seq_t w2  = w * w;
            seq_t w4  = w2 * w2;

            /* Update cumulative sum and store */
            cur_sum += w4;
            a_cache[i] = cur_sum;
        }

        a_cache_size = n + 1;
    }

    return a_cache[n];
}
