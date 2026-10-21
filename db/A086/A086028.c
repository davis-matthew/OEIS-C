/*
 * A086028  a(n) = Sum_{i=1..n} C(i+5,6)^3.
 * https://oeis.org/A086028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} C(i+5,6)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 31, u128 n <= 348,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* binom(idx_t n, idx_t k) returns seq_t */

/* Compute a(n) for any n ≥ 1.
   The result for each n is memoised so that subsequent calls are O(1)
   if the argument is ≤ the largest previously requested n.  */
seq_t a(idx_t n)
{
    static size_t cum_size = 0;   /* number of stored values (0 … cum_size-1) */
    static seq_t *cum      = NULL;   /* cumulative sums: cum[i] = a(i) */

    /* Ensure the array can hold at least n+1 elements (incl. a(0)=0) */
    if (cum_size == 0) {
        cum_size = 1;          /* store a(0) */
        cum = (seq_t *)realloc(cum, cum_size * sizeof(seq_t));
        cum[0] = (seq_t)0;     /* a(0) = 0 – not used but convenient */
    }

    if ((size_t)n >= cum_size) {
        size_t new_size = (size_t)n + 1;
        cum = (seq_t *)realloc(cum, new_size * sizeof(seq_t));

        /* Fill missing entries using the recurrence a(k) = a(k-1) + C(k+5,6)^3 */
        for (size_t i = cum_size; i <= (size_t)n; ++i) {
            idx_t ii = (idx_t)i;
            seq_t c = binom(ii + 5, 6);      /* C(i+5,6) */
            seq_t term = c * c;              /* C^2   */
            term = term * c;                 /* C^3   */
            cum[i] = cum[i - 1] + term;      /* a(i)  */
        }
        cum_size = new_size;
    }

    return cum[(size_t)n];
}
