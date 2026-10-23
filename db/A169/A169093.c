/*
 * A169093  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30 * Σ_{i=1}^{24} a(n-i) − 465 * a(n-25) + b(n), * where
 *            b(0)=1, b(n)=2 for 1 ≤ n ≤ 24, b(25)=1, and b(n)=0 for n≥26. *
 *            Initial value: a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence. */
    static seq_t *cache = NULL;     /* array of computed a(i) values  */
    static size_t cache_len = 0;    /* number of allocated elements   */
    static idx_t max_computed = 0;  /* largest n with a(n) computed   */
    static bool initialized = false;

    size_t target = (size_t)n;         /* index to compute up to        */

    /* First call: allocate the array and compute everything up to target. */
    if (!initialized) {
        cache_len = target + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                 /* allocation failure */

        /* a(0) = 1 */
        cache[0] = (seq_t)1;
        max_computed = 0;

        /* Compute terms 1 .. target. */
        for (size_t i = 1; i <= target; ++i) {
            seq_t sum24 = (seq_t)0;
            for (size_t j = 1; j <= 24 && i >= j; ++j)
                sum24 += cache[i - j];

            seq_t term = (seq_t)30 * sum24;
            if (i >= 25)
                term -= (seq_t)465 * cache[i - 25];

            /* b(i) term */
            if (i <= 24)
                term += (seq_t)2;      /* b(i)=2 for 1..24 */
            else if (i == 25)
                term += (seq_t)1;      /* b(25)=1          */
            /* else b(i)=0 for i>25, nothing to add */

            cache[i] = term;
        }

        initialized = true;
        return cache[n];
    }

    /* If we already have the requested term, just return it. */
    if (target <= (size_t)max_computed)
        return cache[n];

    /* Expand the cached array to have room for target + 1 elements. */
    if (cache_len <= target) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                 /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms, starting from max_computed + 1. */
    for (size_t i = (size_t)max_computed + 1; i <= target; ++i) {
        seq_t sum24 = (seq_t)0;
        for (size_t j = 1; j <= 24 && i >= j; ++j)
            sum24 += cache[i - j];

        seq_t term = (seq_t)30 * sum24;
        if (i >= 25)
            term -= (seq_t)465 * cache[i - 25];

        if (i <= 24)
            term += (seq_t)2;
        else if (i == 25)
            term += (seq_t)1;

        cache[i] = term;
    }

    max_computed = (idx_t)target;
    return cache[n];
}
