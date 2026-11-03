/*
 * A055005  Number of nonnegative integer 3 X 3 matrices with no zero rows or
 *          columns and with sum of elements equal to n.
 * https://oeis.org/A055005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+8,8) - 6*C(n+5,5) + 9*C(n+3,3) + 6*C(n+2,2) -
 *            18*C(n+1,1) + 9
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t t1 = binomial(n + 8, 8);
    seq_t t2 = binomial(n + 5, 5);  t2 *= 6;
    seq_t t3 = binomial(n + 3, 3);  t3 *= 9;
    seq_t t4 = binomial(n + 2, 2);  t4 *= 6;
    seq_t t5 = binomial(n + 1, 1);  t5 *= 18;

    seq_t result = t1;
    result -= t2;
    result += t3;
    result += t4;
    result -= t5;
    result += 9;
    return result;
}
