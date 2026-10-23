/*
 * A168801  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168801:
   a(n) satisfies the linear recurrence
   a(n) = N(n) + 26*∑_{k=1}^{18} a(n-k) - 351*a(n-19),
   where the numerator coefficients N(n) are
   N(0)=1, N(1)…N(18)=2, N(19)=1, and N(n)=0 for n≥20.  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_len = 0;  /* highest index allocated (exclusive) */

/* Return the n‑th term of A168801 (0‑based). */
seq_t a(idx_t n)
{
    /* Grow the cache if n is not yet computed. */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        /* Reallocate to accommodate up to n. */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – ensure no leak and return 0. */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute new terms up to n. */
        for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
            /* Numerator coefficient N(k). */
            seq_t Nk;
            if (k == 0) {
                Nk = (seq_t)1;
            } else if (k <= 18) {
                Nk = (seq_t)2;
            } else if (k == 19) {
                Nk = (seq_t)1;
            } else {
                Nk = (seq_t)0;
            }

            /* Sum of the 18 preceding terms a(k-1)…a(k-18). */
            seq_t sum18 = (seq_t)0;
            for (idx_t i = 1; i <= 18 && k >= i; ++i) {
                sum18 += cache[k - i];
            }

            /* Term from 19 positions back, if it exists. */
            seq_t a19 = (k >= 19) ? cache[k - 19] : (seq_t)0;

            /* Compute the current term using the recurrence. */
            cache[k] = Nk + (26 * sum18) - (351 * a19);
        }
        cache_len = new_len;
    }

    return cache[n];
}
