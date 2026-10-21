/*
 * A062098  a(n) = 7 * n!.
 * https://oeis.org/A062098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Store factorials in a cache so that multiple calls stay fast. */
    static seq_t *fact = NULL;          /* factorial cache: fact[k] = k! */
    static size_t fact_cap = 0;         /* current capacity of the cache */
    static idx_t fact_len = 0;          /* number of factorials stored (>=1) */

    /* Initalise cache with 0! = 1 */
    if (!fact)
    {
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) abort();             /* OOM protection */
        fact[0] = (seq_t)1;             /* 0! = 1 */
        fact_len = 1;
        fact_cap = 1;
    }

    /* Expand cache if necessary (ensure index n is available) */
    if ((idx_t)(fact_cap - 1) < n)
    {
        /* Grow to at least n+1 */
        size_t needed = (size_t)n + 1;
        size_t newcap = fact_cap ? fact_cap << 1 : 16;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact, newcap * sizeof(seq_t));
        if (!tmp) abort();              /* OOM protection */
        fact = tmp;
        fact_cap = newcap;
    }

    /* Compute new factorials up to n if they haven't been computed yet */
    if (n >= fact_len)
    {
        for (idx_t i = fact_len; i <= n; ++i)
        {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = n + 1;
    }

    /* Return the required term 7 * n! */
    return (seq_t)7 * fact[n];
}
