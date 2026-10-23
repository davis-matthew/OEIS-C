/*
 * A170397  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For 1 <= n <= 43 : a(n) = 2 + 2*sum_{k=0}^{n-1} a(k).
 *            For n = 44 : a(44) = 2*sum_{k=1}^{43} a(k) - 2. For n >= 45 :
 *            a(n) = 2*sum_{k=n-43}^{n-1} a(k) - 3*a(n-44)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cap     = 0;        /* allocated capacity */
    static idx_t  last    = (idx_t)-1;/* last index with computed value */
    static seq_t cum      = 0;        /* cumulative sum up to last index         */
    static seq_t win_sum = 0;         /* sliding window sum for k >= 44           */

    /* Ensure storage for requested index */
    if (n + 1 > cap) {
        size_t newcap = cap == 0 ? 64 : cap << 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* First call: initialise a(0) */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1; /* a(0) = 1 */
        cum      = cache[0];
        last     = 0;
    }

    if (n <= last) {
        return cache[n];
    }

    /* Compute terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t val;

        if (k <= (idx_t)43) {
            /* a(k) = 2 + 2*sum_{i=0}^{k-1} a(i) */
            val = (seq_t)2 + (seq_t)2 * cum;
            cache[k] = val;
            cum += val;
        } else if (k == (idx_t)44) {
            /* After computing up to 43, cum = sum_{i=0}^{43} a(i)
               win_sum holds sum_{i=1}^{43} a(i) */
            win_sum = cum - cache[0];                 /* sum_{i=1}^{43} */
            val = (seq_t)2 * win_sum - (seq_t)2;      /* a(44) formula */
            cache[k] = val;
            /* Prepare win_sum for next iteration */
            win_sum = win_sum + val - cache[1];       /* sum_{i=2}^{44} */
        } else { /* k >= 45 */
            /* a(k) = 2*sum_{i=k-43}^{k-1} a(i) - 3*a(k-44) */
            val = (seq_t)2 * win_sum - (seq_t)3 * cache[k - (idx_t)44];
            cache[k] = val;
            /* Update sliding window for next step */
            win_sum = win_sum + val - cache[k - (idx_t)43];
        }
    }

    last = n;
    return cache[n];
}
