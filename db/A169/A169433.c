/*
 * A169433  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 34*Σ_{k=1}^{31}a(n-k) – 595*a(n-32) where p(0)=1,
 *            p(1..31)=2, p(32)=1, p(n)=0 for n>32
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;    /* allocated capacity (in seq_t elements) */
    static idx_t   first  = 0;    /* next index that will be computed */

    /* grow the cache if necessary */
    if (n >= cap)
    {
        size_t newcap = (cap == 0) ? 1 : cap;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();           /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute values from 'first' up to n */
    for (idx_t i = first; i <= n; ++i)
    {
        seq_t sum = 0;                     /* 34*Σ a(i-k) */
        idx_t up = (i < 31 ? (idx_t)i : 31);
        for (idx_t k = 1; k <= up; ++k)
            sum += cache[i - k] * (seq_t)34;

        /* p(i) */
        seq_t term = 0;
        if (i == 0)
            term = 1;
        else if (i <= 31)             /* i in 1..31 */
            term = 2;
        else if (i == 32)
            term = 1;

        seq_t val = sum + term;
        if (i >= 32)
            val -= 595 * cache[i - 32];

        cache[i] = val;
    }
    first = n + 1;  /* next value to compute will be n+1 */

    return cache[n];
}
