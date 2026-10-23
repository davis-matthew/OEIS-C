/*
 * A170086  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170086
   Recurrence derived from the rational generating function:
      a(0) = 1
      a(n) = p(n) + 27 * Σ_{i=1}^{min(n,36)} a(n-i)
                  - (n≥37 ? 378 * a(n-37) : 0)
   where
      p(0) = 1 , p(1..36) = 2 , p(37) = 1 , otherwise p(n)=0
   The implementation uses memoisation in a statically allocated buffer
   large enough for the required range (n ≤ 10 000).  Calls are
   independent of order and run in O(n) time per call.  All arithmetic
   is performed in the user‑supplied type seq_t.
*/

#include <stddef.h>   /* for size_t */

static seq_t a_cache[10001];  /* buffer for computed values */
static idx_t a_cached_len = 0;/* number of values stored (a_cache[0..len-1]) */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* if the value has already been computed, return it */
    if (n < a_cached_len)
        return a_cache[n];

    /* compute values up to n, storing them in the cache */
    for (idx_t i = a_cached_len; i <= n; ++i)
    {
        seq_t sum = 0;        /* 27 * Σ a(i-j) */
        idx_t lim = i < 36 ? i : 36;
        for (idx_t j = 1; j <= lim; ++j)
        {
            sum += a_cache[i - j] * (seq_t)27;
        }

        /* p(i) contribution */
        seq_t p;
        if (i == 0)
            p = (seq_t)1;
        else if (i <= 36)
            p = (seq_t)2;
        else if (i == 37)
            p = (seq_t)1;
        else
            p = (seq_t)0;

        seq_t val = sum + p;

        /* subtract the 378 * a(i-37) term when applicable */
        if (i >= 37)
        {
            val -= a_cache[i - 37] * (seq_t)378;
        }

        a_cache[i] = val;
    }

    a_cached_len = n + 1;
    return a_cache[n];
}
