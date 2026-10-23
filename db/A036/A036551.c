/*
 * A036551  a(n) = 2^(n-1)*(3^n-1) + 1.
 * https://oeis.org/A036551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(3^n-1)+1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow2n = pow2(n - 1);
    seq_t pow3n = pow3(n);
    seq_t temp = pow3n;
    temp -= (seq_t)1;
    temp *= pow2n;
    temp += (seq_t)1;
    return temp;
}
