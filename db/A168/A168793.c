/*
 * A168793  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in a Coxeter group on
 *            20 generators with relations (S_i)^2 = (S_i S_j)^19 = I. The
 *            sequence satisfies the recurrence derived from its generating
 *            function: a(0)=1, a(n)=2+18*∑_{k=0}^{n-1}a(k) for 1 ≤ n ≤ 18,
 *            a(19)=18*∑_{k=0}^{18}a(k)−188, a(n)=18*∑_{k=n-1...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, exit */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;
    static seq_t  running_sum = 0;   /* sum of cache[0..cache_len-1] (only used up to n≤18) */

    /* Ensure sufficient capacity for the cache. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n. */
    while (cache_len <= n) {
        idx_t i = cache_len;
        seq_t val;

        if (i == 0) {
            val = 1;
            running_sum = 1;
        } else if (i <= 18) {
            val = (seq_t)2 + (seq_t)18 * running_sum;
            running_sum += val;
        } else if (i == 19) {
            /* running_sum now equals sum of a[0..18] */
            val = (seq_t)18 * running_sum - (seq_t)188;
            running_sum += val;   /* no longer needed after i>=19 */
        } else {
            /* Sum the last 18 values. */
            seq_t window = 0;
            for (idx_t j = i - 18; j <= i - 1; ++j)
                window += cache[j];
            val = (seq_t)18 * window - (seq_t)171 * cache[i - 19];
            /* running_sum is not used beyond this point, but keep updating for consistency */
            running_sum += val;
        }

        cache[i] = val;
        ++cache_len;
    }

    return cache[n];
}
