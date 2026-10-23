/*
 * A101913  G.f. satisfies: A(x) = 1/(1 + x*A(x^3)) and also the continued
 *          fraction: 1+x*A(x^4) =
 *          [1;1/x,1/x^3,1/x^9,1/x^27,...,1/x^(3^(n-1)),...].
 * https://oeis.org/A101913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Sum_{k=0..floor((n-1)/3)} a(k) * a(n - 3*k - 1) a(0) =
 *            1. The function evaluates the recurrence directly for each call,
 *            recomputing all preceding terms up to n
 * Range:     i8 n <= 14, i16 n <= 29, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate space for all terms up to n.  */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (seq == NULL) abort();           /* Out of memory */

    seq[0] = (seq_t)1;                  /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxk = (i > 0) ? (i - 1) / 3 : 0;

        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t j = i - 3 * k - 1;    /* second factor index */
            sum += seq[k] * seq[j];
        }
        seq[i] = -sum;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
