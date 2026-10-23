/*
 * A194455  a(n) = 2^n + 3n + 1.
 * https://oeis.org/A194455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 3*n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow = pow2(n);

    /* compute 3*n + 1 */
    seq_t term = ((seq_t)3 * (seq_t)n) + (seq_t)1;

    return pow + term;
}
