/*
 * A100098  An inverse Chebyshev transform of (1-x)/(1-2x).
 * https://oeis.org/A100098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k)*2^(n-2k-1) , with the
 *            special term binomial(n,n/2) added when n is even
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   binomial, seq_malloc, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation caches previous values and computes missing terms
   using the binomial recurrence to avoid large intermediate products.  */

#include <stddef.h>            /* for size_t */
#include "../utils/binomial.h" /* seq_t binomial(idx_t N, idx_t K) */
#include "../utils/seq_pow.h"  /* seq_t seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_malloc.h" /* seq_t *seq_malloc(idx_t n) */
#include "../utils/seq_realloc.h"/* seq_t *seq_realloc(seq_t *p, idx_t n) */

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;   /* cached sequence values  */
    static idx_t memo_size = 0;  /* length of the cache array */

    /* Ensure the cache is large enough to hold the requested index.  */
    if (memo == NULL) {
        idx_t cap = (n + 1 > 3 ? n + 1 : 3);      /* at least 3 entries  */
        memo = seq_malloc(cap);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)4;
        memo_size = 3;
    }
    else if (n + 1 > memo_size) {
        /* Grow the array to accommodate the new index.  */
        idx_t old_size = memo_size;
        idx_t new_size = n + 1;

        memo = seq_realloc(memo, new_size);
        memo_size = new_size;           /* array length after reallocate  */

        /* Compute the new terms that are missing in the cache.  */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i < 3) continue;        /* already initialized  */

            idx_t maxk = i / 2;         /* floor(i/2)  */
            seq_t bin = (seq_t)1;       /* binomial(i,0)  */
            seq_t sum = (seq_t)0;

            for (idx_t k = 0; k <= maxk; ++k) {
                if (i == 2 * k) {       /* special even‑index contribution */
                    sum += bin;
                } else {
                    idx_t shift = i - 2 * k - 1;       /* exponent of 2  */
                    seq_t pow2 = seq_pow((seq_t)2, shift);
                    sum += bin * pow2;
                }

                /* Update binomial coefficient for the next k.  */
                if (k < maxk) {
                    /* bin = bin * (i - k) / (k + 1)  (exact division).  */
                    bin = (bin * (seq_t)(i - k)) / (seq_t)(k + 1);
                }
            }

            memo[i] = sum;
        }
    }

    return memo[n];
}
