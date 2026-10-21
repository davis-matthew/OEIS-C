/*
 * A087288  a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3), with initial terms 4, 4, 10.
 * https://oeis.org/A087288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1)+1+(-1)^n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n + 1);
    if ((n & 1ULL) == 0) {  /* n even  */
        val += 2;
    }
    return val;
}
