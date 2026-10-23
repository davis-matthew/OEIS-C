/*
 * A174112  After correction, duplicate of A111490.
 * https://oeis.org/A174112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Sum_{i=1..n} (2*i - sigma(i)) = n*(n+1) - Sum_{i=1..n}
 *            sigma(i)
 * Range:     i8 n <= 24, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(i) = sum of divisors of i.  The function recomputes the
 * required values on each call, using an O(n log n) divisor sieve. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined offset, safe guard */

    /* Allocate array for sigma[1..n], sigma[0] unused. */
    seq_t *sigma = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!sigma) return (seq_t)0;                /* allocation failed */

    /* initialize sigma array to zero */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = (seq_t)0;

    /* Sieve to compute sigma for all numbers up to n. */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t dval = (seq_t)d;
        for (idx_t m = d; m <= n; m += d) {
            sigma[m] += dval;
        }
    }

    /* Compute the cumulative sum a(n). */
    seq_t result = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t iv = (seq_t)i;
        result += (2 * iv) - sigma[i];
    }

    free(sigma);
    return result;
}
