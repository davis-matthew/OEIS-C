/*
 * A168905  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:  a(0)=1 ,  a(1)=36 ,  a(n)=35·a(n-1)  for 2≤n≤20
    For n≥21:  a(n)=595·a(n-21)+34·(a(n-1)+a(n-2)+⋯+a(n-20)) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static dynamic cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Grow cache if required */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the term, just return it */
    if (n < cache_len)
        return cache[n];

    /* Sliding sum of the last 20 terms (for n ≥ 21) */
    seq_t sum20 = 0;   /* will be initialized when we hit n==21 */

    /* Compute missing terms */
    for (idx_t j = cache_len; j <= n; ++j) {
        if (j == 0) {
            cache[0] = (seq_t)1;
        } else if (j == 1) {
            cache[1] = (seq_t)36;
        } else if (j <= 20) {
            cache[j] = cache[j - 1] * (seq_t)35;
        } else { /* j ≥ 21 */
            if (j == 21) {  /* initialise sliding sum for 1..20 */
                sum20 = 0;
                for (idx_t k = 1; k <= 20; ++k)
                    sum20 += cache[k];
            } else {
                /* Update sliding window: add a_{j-1}, drop a_{j-21} */
                sum20 += cache[j - 1];
                sum20 -= cache[j - 21];
            }
            seq_t part1 = (seq_t)595 * cache[j - 21];
            seq_t part2 = (seq_t)34 * sum20;
            cache[j] = part1 + part2;
        }
    }

    /* Update the length of cached terms */
    cache_len = (idx_t)(n + 1);

    return cache[n];
}
