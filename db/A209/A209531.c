/*
 * A209531  Half the number of (n+1) X 5 0..2 arrays with every 2 X 2 subblock
 *          having exactly two distinct clockwise edge differences.
 * https://oeis.org/A209531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n is odd: 2^((5*(n+1))/2) + 1 if n is even:
 *            3*2^((5*n)/2 + 1) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0
       (the value is irrelevant because the offset is 1). */
    if (n == 0)
        return 0;

    if (n & 1ULL) {                    /* n odd */
        idx_t exponent = (5 * (n + 1)) / 2;
        seq_t val = pow2(exponent);    /* 2^exponent */
        return val + 1;
    } else {                           /* n even */
        idx_t exponent = (5 * n) / 2 + 1;
        seq_t val = pow2(exponent);    /* 2^exponent */
        val = val * 3;                 /* 3 * 2^exponent */
        return val + 1;
    }
}
