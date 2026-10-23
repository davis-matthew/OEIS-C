/*
 * A027982  a(n) = Sum_{k=0..2*n} (k+1)*T(n, 2*n-k), T given by A027960.
 * https://oeis.org/A027982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 + 2*n) * 2^n - (10 + 6*n)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n as seq_t */
    seq_t pow2n = pow2(n);

    /* factor = 11 + 2*n */
    seq_t factor = (seq_t)11 + ((seq_t)2 * (seq_t)n);

    /* term = factor * 2^n */
    seq_t term = pow2n * factor;

    /* minus = 10 + 6*n */
    seq_t minus = (seq_t)10 + ((seq_t)6 * (seq_t)n);

    seq_t result = term - minus;
    return result;
}
