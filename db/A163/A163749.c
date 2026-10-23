/*
 * A163749  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1)+43*a(n-2)+43*a(n-3)+43*a(n-4)-946*a(n-5)
 *            Initial values: a(0)=1, a(1)=45, a(2)=1980, a(3)=87120,
 *            a(4)=3833280, a(5)=168663330 The recurrence applies for n >= 6.
 *            The first six terms are treated as given
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_len = 0;     /* last index computed + 1 */
static size_t cache_cap = 0;     /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Ensure cache has capacity up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = cache_len; (size_t)i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)45;
        } else if (i == 2) {
            val = (seq_t)1980;
        } else if (i == 3) {
            val = (seq_t)87120;
        } else if (i == 4) {
            val = (seq_t)3833280;
        } else if (i == 5) {
            val = (seq_t)168663330;
        } else {
            /* recurrence for n >= 6 */
            seq_t t = cache[i-1] * (seq_t)43
                    + cache[i-2] * (seq_t)43
                    + cache[i-3] * (seq_t)43
                    + cache[i-4] * (seq_t)43
                    - cache[i-5] * (seq_t)946;
            val = t;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
