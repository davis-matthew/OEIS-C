/*
 * A170519  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)   = 1
   For 1 ≤ n ≤ 45:   a(n) = 2 + 28 * Σ_{k=1}^{n} a(n−k)
   a(46)  = 1 + 28 * Σ_{k=1}^{45} a(46−k) − 406 * a(0)
   For n ≥ 47:       a(n) = 28 * Σ_{k=1}^{45} a(n−k) − 406 * a(n−46)   */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* The sequence is computed on demand and cached forever.
       A static array of size 20 001 suffices for all calls with
       n up to about 10 000 (the test range). */
    static seq_t cache[20001];
    static idx_t last_computed = (idx_t)-1;   /* largest index already computed */

    /* If the cache is empty, compute the first term. */
    if (last_computed == (idx_t)-1)
    {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* Extend the cache up to n if necessary. */
    if (n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            /* Sum the previous up to 45 terms. */
            seq_t sumPrev = 0;
            for (idx_t j = 1; j <= 45 && j <= i; ++j)
                sumPrev += cache[i - j];

            seq_t val = 0;
            if (i == 0)
                val = (seq_t)1;
            else if (i <= 45)          /* 1 ≤ i ≤ 45 */
                val = (seq_t)2 + (seq_t)28 * sumPrev;
            else if (i == 46)          /* a(46) */
            {
                /* sumPrev already contains a(1)..a(45) */
                val = (seq_t)1 + (seq_t)28 * sumPrev - (seq_t)406 * cache[0];
            }
            else                        /* i ≥ 47 */
            {
                /* sumPrev contains a(i-1)..a(i-45) */
                val = (seq_t)28 * sumPrev - (seq_t)406 * cache[i - 46];
            }

            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
