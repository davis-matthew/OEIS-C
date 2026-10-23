/*
 * A144773  10-fold factorials: Product_{k=0..n-1} (10*k+1).
 * https://oeis.org/A144773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (10*k + 1) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS sequence A144773.
 *  The sequence is defined as the product of the numbers
 *  10*k+1 for k = 0 … n‑1.  For n = 0 the product is empty
 *  and the value is 1.
 *
 *  The function performs the multiplication iteratively and
 *  does not use any caching, so it can be called an arbitrary
 *  number of times with any order of indices.
 */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) = 1 */
    for (idx_t k = 0; k < n; ++k) {
        seq_t factor = (seq_t)(10 * k + 1);
        result = result * factor;
    }
    return result;
}
