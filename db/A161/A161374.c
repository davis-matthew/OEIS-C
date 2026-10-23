/*
 * A161374  "Punctual" binary numbers. Complement of A161373.
 * https://oeis.org/A161374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n>=9, a(3q)=2^(2q-1), a(3q+1)=2^(2q-1)+2^(q-1),
 *            a(3q+2)=2^(2q). * For n<=8 the values are: 0,1,2,4,8,10,16,22
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 8) {
        switch ((uint32_t)n) {
            case 1:  return 0;
            case 2:  return 1;
            case 3:  return 2;
            case 4:  return 4;
            case 5:  return 8;
            case 6:  return 10;
            case 7:  return 16;
            case 8:  return 22;
            default: return 0; /* unreachable */
        }
    }

    idx_t q   = n / 3;
    idx_t rem = n % 3;

    seq_t pow1 = pow2(2 * q - 1); /* 2^(2q-1) */

    switch (rem) {
        case 0: /* n = 3q */
            return pow1;
        case 1: /* n = 3q+1 */
        {
            seq_t pow2_q = pow2(q - 1); /* 2^(q-1) */
            return pow1 + pow2_q;
        }
        case 2: /* n = 3q+2 */
            return pow2(2 * q); /* 2^(2q) */
        default:
            return 0; /* unreachable */
    }
}
