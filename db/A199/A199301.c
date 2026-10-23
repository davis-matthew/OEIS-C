/*
 * A199301  a(n) = (2n+1)*8^n.
 * https://oeis.org/A199301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)*8^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* (2n+1) as a seq_t value */
    seq_t two_n_plus_one = ((seq_t)n * (seq_t)2) + (seq_t)1;

    /* 8^n in a seq_t value */
    seq_t pow8 = pow_int((seq_t)8, n);

    /* final result */
    return two_n_plus_one * pow8;
}
