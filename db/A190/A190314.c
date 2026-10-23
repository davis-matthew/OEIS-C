/*
 * A190314  The number of cycles in the digraph representation of all
 *          endofunctions on {1,2,...,n}.
 * https://oeis.org/A190314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k-1)! * C(n,k) * n^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the recurrence
   term_1 = n^n
   term_k = term_{k-1} * ((k-1)*(n-k+1))/(k*n)   for k>=2
   and sums all terms.  This runs in O(n) big‑integer
   operations and works for all n for which the
   sequence values fit in the supplied seq_t type. */

#include <stdbool.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    seq_t term   = 0;
    const seq_t n_seq = (seq_t)n;

    /* term for k = 1 : n^n */
    term = pow_int(n_seq, n);
    result = term;

    /* Iterate for k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        const seq_t mult_left  = (seq_t)(k - 1);        /* (k-1) */
        const seq_t mult_right = (seq_t)(n - k + 1);    /* (n-k+1) */
        const seq_t denom      = (seq_t)k * n_seq;      /* k*n */

        /* term = term * (k-1)*(n-k+1) / (k*n) */
        term *= mult_left;
        term *= mult_right;
        term /= denom;

        result += term;
    }

    return result;
}
