/*
 * A126643  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digits 1,2,3,4, at least one
 *          of digits 5,6 and at least one of digits 7,8,9.
 * https://oeis.org/A126643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*4^n - 9*3^n + 5*2^n - 1, n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1, return 0 for safety */
        return (seq_t)0;

    /* Compute the three powers */
    seq_t p4 = seq_pow((seq_t)4, n);
    seq_t p3 = seq_pow((seq_t)3, n);
    seq_t p2 = seq_pow((seq_t)2, n);

    /* Combine them according to the formula */
    seq_t result = 6 * p4;
    result -= 9 * p3;
    result += 5 * p2;
    result -= (seq_t)1;

    return result;
}
