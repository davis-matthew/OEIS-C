/*
 * A049435  Stirling numbers of second kind: 10th column of Stirling2 triangle
 *          A008277.
 * https://oeis.org/A049435
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 17, u64 n <= 26, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A049435
 *  a(n) = Stirling2(n,10)
 *  (10th column of Stirling numbers of the second kind)
 *  Offset: 10
 *
 *  Implementation uses dynamic programming for recurrence
 *        S(0,0)=1,  S(n,0)=0 for n>0
 *        S(n,k)=S(n-1,k-1)+k*S(n-1,k)
 *  The function is memoised so that repeated calls in any order are
 *  answered in O(1) after the values up to the maximum requested n
 *  have been computed.  This works for any seq_t type (plain integer,
 *  128‑bit integer, or an arbitrary‑precision integer class provided
 *  it can be default constructed and assigned) because the code only
 *  uses simple assignments, additions and multiplications.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n)=S(n,10) for n>=10; returns 0 for n<10 */
seq_t a(idx_t n)
{
    if (n < 10) return 0;                   /* defined only for n>=10 */

    /* Static state: memoisation cache and DP array */
    static seq_t *cache = NULL;          /* cache[i] = a(i) for 0<=i<=cache_upto */
    static idx_t cache_size = 0;
    static idx_t cache_upto = 0;

    static seq_t dp[11];                /* dp[k] = S(current_n, k) */
    static bool dp_inited = false;

    /* initialise DP base */
    if (!dp_inited) {
        dp[0] = 1;
        for (unsigned int k = 1; k <= 10; ++k)
            dp[k] = 0;
        dp_inited = true;
    }

    /* initialise cache on first call */
    if (!cache) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure – unlikely */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;                /* safe initialisation for any seq_t */
        cache_upto = 0;
    }

    /* extend precomputation up to n if necessary */
    if (n > cache_upto) {
        for (idx_t i = cache_upto + 1; i <= n; ++i) {
            seq_t newdp[11];
            /* zero newdp */
            for (unsigned int k = 0; k <= 10; ++k)
                newdp[k] = 0;

            /* compute S(i,k) for k=1..min(i,10) */
            unsigned int maxk = (i < 10 ? (unsigned int)i : 10);
            for (unsigned int k = 1; k <= maxk; ++k) {
                newdp[k] = dp[k-1] + (seq_t)k * dp[k];
            }

            /* copy newdp to dp for next iteration */
            for (unsigned int k = 0; k <= 10; ++k)
                dp[k] = newdp[k];

            /* resize cache if required */
            if (i >= cache_size) {
                size_t new_cap = cache_size ? cache_size : 16;
                while (new_cap <= (size_t)i)
                    new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;               /* allocation failure */
                /* zero the newly allocated part */
                for (size_t idx = cache_size; idx < new_cap; ++idx)
                    tmp[idx] = 0;
                cache = tmp;
                cache_size = new_cap;
            }

            /* store result  S(i,10) in cache */
            cache[i] = dp[10];
        }
        cache_upto = n;
    }

    return cache[n];
}
