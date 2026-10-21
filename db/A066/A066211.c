/*
 * A066211  a(n) = Sum_{j=0..n} (2*n)!/(2*n-j)!.
 * https://oeis.org/A066211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (2*n)!/(2*n-j)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The j = 0 summand is 1. */
    seq_t sum  = 1;
    seq_t term = 1;

    /* Compute 2*n in the index type to avoid overflow of seq_t
       in cases where seq_t is smaller than idx_t. */
    idx_t twon = n * 2;

    /* Accumulate the remaining summands. */
    for (idx_t j = 1; j <= n; ++j) {
        /* (2*n - j + 1) fits into idx_t; cast to seq_t for the
           multiplication.  The cast may wrap for very small seq_t,
           but that is acceptable because such values are beyond the
           range where the sequence fits in seq_t. */
        term *= (seq_t)(twon - j + 1);
        sum += term;
    }

    return sum;
}
