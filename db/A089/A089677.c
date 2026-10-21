/*
 * A089677  Exponential convolution of A000670(n), with A000670(0)=0, with the
 *          sequence of all ones alternating in sign.
 * https://oeis.org/A089677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A089677
    Formula:  a(0) = 0
              a(n) = Σ_{k=0}^{n-1} C(n,k) * a(k) + (-1)^{n+1}   (n ≥ 1)
             where C(n,k) is the binomial coefficient.
    The function memoises already computed values so that calls in any
    order are O(n) overall.                                             */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache   = NULL;   /* cache[i] holds a(i)  */
    static size_t cached  = 0;     /* number of cached entries */

    /* Allocate the first entry if necessary */
    if (cached == 0) {
        cache  = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – should not happen */
        cache[0] = (seq_t)0;           /* a(0) */
        cached  = 1;
    }

    /* Grow the cache up to n if needed */
    if ((size_t)n + 1 > cached) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cached; i <= (idx_t)n; ++i) {
            if (i == 0) {            /* a(0) already set */
                continue;
            }
            seq_t sum  = (seq_t)0;
            seq_t binom = (seq_t)1;   /* C(i,0) */

            /* Σ_{k=0}^{i-1} C(i,k) * a(k) */
            for (idx_t k = 0; k < i; ++k) {
                sum += binom * cache[k];

                /* Update binom to C(i, k+1)  */
                seq_t num = (seq_t)(i - k);   /* (i - k) */
                seq_t den = (seq_t)(k + 1);   /* (k + 1) */
                binom = (binom * num) / den;
            }

            /* Add (-1)^{i+1} */
            if ((i & (idx_t)1) == 0) {   /* i even → (-1)^{i+1} = -1 */
                sum -= (seq_t)1;
            } else {                     /* i odd  → (+1)              */
                sum += (seq_t)1;
            }

            cache[i] = sum;
        }
        cached = new_len;
    }

    return cache[(size_t)n];
}
