/*
 * A166254  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=42∑_{i=1..9}a(n-i) – 903·a(n-10) + q(n) where
 *            q(0)=1, q(1)..q(9)=2, q(10)=1, q(n>10)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation memoises computed values up to the largest
   index requested so far.           */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static size_t capacity    = 0;   /* allocated array size          */
    static size_t cached      = 0;   /* elements currently computed   */

    /* Convert idx_t to size_t for indexing (n is unsigned) */
    size_t idx = (size_t)n;

    /* Grow cache if needed */
    if (idx >= cached)
    {
        size_t newlen = idx + 1;
        if (newlen > capacity)
        {
            size_t newcap = capacity ? capacity << 1 : 1;
            while (newcap < newlen)
                newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                abort();                 /* allocation failed */
            cache = tmp;
            capacity = newcap;
        }

        /* Compute missing values */
        for (size_t i = cached; i < newlen; ++i)
        {
            /* rhs constant q(i) */
            seq_t q = 0;
            if (i == 0)
                q = 1;
            else if (i <= 9)
                q = 2;
            else if (i == 10)
                q = 1;

            /* 42 * sum_{j=1..9} a(i-j) */
            seq_t sum = 0;
            size_t ub = (i < 9) ? i : 9;      /* number of terms to sum */
            for (size_t j = 1; j <= ub; ++j)
                sum += cache[i - j];

            seq_t val = q + (seq_t)42 * sum;
            if (i >= 10)
                val -= (seq_t)903 * cache[i - 10];

            cache[i] = val;
        }
        cached = newlen;
    }
    return cache[idx];
}
