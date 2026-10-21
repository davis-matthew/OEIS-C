/*
 * A082144  A subdiagonal of number array A082137.
 * https://oeis.org/A082144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ( 2^(n-1) + 0^n/2 ) * C(2*n+2, n) (with 0^0 = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 separately (0^0 = 1) */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Binomial coefficient C(2n+2, n) */
    seq_t bin = binom(2*n + 2, n);

    /* 2^(n-1) – computed by shifting 1 left (n-1) times */
    seq_t pow2 = ((seq_t)1) << (n - 1);

    /* Result */
    return bin * pow2;
}
