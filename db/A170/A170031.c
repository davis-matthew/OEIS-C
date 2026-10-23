/*
 * A170031  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=22*21^(n-1) for n>=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Since n is guaranteed to be small (≤ 10000 for the tests),
       it safely fits into an int for the exponent of 21. */
    int exp = (int)(n - 1);

    seq_t pow21 = pow_seq((seq_t)21, exp);

    return pow21 * (seq_t)22;
}
