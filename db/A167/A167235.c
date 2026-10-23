/*
 * A167235  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   free_seq_array, p_val, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A167235
 *  Recurrence derived from the rational generating function
 *
 *      G(t) = (t^14 + 2*t^13 + 2*t^12 + … + 2*t + 1) /
 *             (351*t^14 - 26*t^13 - 26*t^12 - … - 26*t + 1)
 *
 *  Let p_n be the numerator coefficients:
 *      p_0 = 1,  p_1 … p_13 = 2,  p_14 = 1,  p_n = 0 for n > 14.
 *
 *  The recurrence for n ≥ 0 is
 *
 *      a[n] = p_n
 *           + 26 * ∑_{k=1}^{13} a[n-k]
 *           - 351 * a[n-14]              (with a[m] = 0 for m < 0)
 *
 *  This produces the listed terms:
 *      a(0)=1, a(1)=28, a(2)=756, a(3)=20412, …
 *
 *  The implementation memoises previously computed values so that
 *  calls can be made in any order.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/p_val.h"

/* Cached values of the sequence */
static seq_t *cache   = NULL;
static idx_t  cap     = 0;

/* Return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n+1 elements */
    if (n >= cap) {
        idx_t new_cap = n + 1;

        /* Allocate a new cache array large enough for {0 … n} */
        seq_t *new_cache = seq_alloc_array(new_cap);

        /* Copy previously computed values */
        for (idx_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];

        /* Compute values from cap up to n (inclusive) */
        for (idx_t i = cap; i <= n; ++i) {
            seq_t temp = p_val(i);                 /* start with p[i] */

            /* Add 26 * a[i-k] for k = 1 … 13 if defined */
            if (i >= 1) {
                for (idx_t k = 1; k <= 13 && i >= k; ++k)
                    temp += new_cache[i - k] * (seq_t)26;
            }

            /* Subtract 351 * a[i-14] if defined */
            if (i >= 14)
                temp -= new_cache[i - 14] * (seq_t)351;

            new_cache[i] = temp;
        }

        /* Free old cache and replace it with the new one */
        if (cache != NULL)
            free_seq_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[n];
}
