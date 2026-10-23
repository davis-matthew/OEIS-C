/*
 * A167212  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1. For n≥1 a(n) = rhs[n] + 23·(a(n−1)+…+a(n−13)) −
 *            276·a(n−14), where rhs[0] = 1 , rhs[1]…rhs[13] = 2 , rhs[14] = 1
 *            , else 0. This recurrence is derived from the rational
 *            generating function of A167212. The sequence is memoised so any
 *            order of calls is handled efficiently. Recall that seq_t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;   /* allocated capacity in elements     */
static size_t cache_len = 0;   /* number of elements that are valid  */

/* ensure that terms 0 .. n are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)         /* already computed                */
        return;

    size_t needed = (size_t)n + 1;
    if (cache_cap < needed)    /* grow array if necessary          */
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len up to n */
    for (size_t m = cache_len; m <= (size_t)n; ++m)
    {
        seq_t rhs = 0;
        if (m == 0)           rhs = 1;
        else if (m <= 13)      rhs = 2;
        else if (m == 14)      rhs = 1;   /* rhs[n] = 1 if n == 14 */

        seq_t sum = rhs;

        /* add 23 times the previous 13 terms */
        for (int i = 1; i <= 13; ++i)
            if (m >= (size_t)i)
                sum += 23 * cache[m - i];

        /* subtract 276 times the term 14 steps back */
        if (m >= 14)
            sum -= 276 * cache[m - 14];

        cache[m] = sum;
    }

    cache_len = (size_t)n + 1;
}

/* external linkage: returns a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
