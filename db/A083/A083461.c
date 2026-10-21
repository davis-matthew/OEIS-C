/*
 * A083461  Palindromes arising in A083457.
 * https://oeis.org/A083461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A083457(n)^n - 1 where A083457(n) is 1 for n=1, 2 for n=2
 *            or n=3, 10 for n=4 or n>=6, 3 for n=5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case n == 1: 1^1 - 1 = 0 */
    if (n == 1)
        return (seq_t)0;

    seq_t base;
    switch (n) {
        case 2:
        case 3:
            base = (seq_t)2;
            break;
        case 4:
            base = (seq_t)10;
            break;
        case 5:
            base = (seq_t)3;
            break;
        default:                       /* n >= 6 */
            base = (seq_t)10;
            break;
    }

    /* Compute base^n - 1 */
    seq_t power = big_pow(base, (seq_t)n);
    return power - (seq_t)1;
}
