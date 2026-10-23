/*
 * A170388  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n>0: if 1<=n<=42 : N(n)=2 if n==43 : N(n)=1 else :
 *            N(n)=0 Recurrence: a(n) = 41*∑_{i=1}^{42} a(n-i) - 861*a(n-43) +
 *            N(n) (The sums use only terms already computed; for n<43 terms
 *            with negative indices are treated as 0.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function computes the sequence in increasing order of n, memoising
    all values in a static array.  It works for any seq_t supported by the
    build (including an arbitrary‑precision integer class).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored */

    /* If we need more values, grow the cache.  */
    if (n >= cache_len)
    {
        /* Allocate space for all terms up to n.  */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;   /* memory allocation failed */

        cache = tmp;

        /* Compute all missing terms, starting from the first uncomputed one. */
        for (idx_t k = cache_len; k <= n; ++k)
        {
            /* sum_{i=1}^{42} a(k-i)  (treat non‑existent indices as 0) */
            seq_t sum = 0;
            for (idx_t d = 1; d <= 42 && k >= d; ++d)
                sum += cache[k - d];

            seq_t val = sum * 41;           /* 41 * sum */
            if (k >= 43)
                val -= cache[k - 43] * 861; /* subtract 861 * a(k-43) */

            /* add the N(k) term */
            seq_t Nk = 0;
            if (k == 0)
                Nk = 1;
            else if (k <= 42)
                Nk = 2;
            else if (k == 43)
                Nk = 1;
            val += Nk;

            cache[k] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
