/*
 * A115125  A sequence related to Catalan numbers A000108.
 * https://oeis.org/A115125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n-1) * 2^n (a(0) := 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

/* Computes the n-th term of OEIS A115125. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return catalan(n - 1) * pow2(n);
}
