/*
 * A120926  Number of isolated 0's in all ternary words of length n on
 *          {0,1,2}.
 * https://oeis.org/A120926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==1?1:4*(n+1)*3^n/27)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined; harmless fallback */
    if (n == 1) return 1;

    /* 3^n */
    seq_t p3 = pow3(n);

    /* (n+1)*3^n */
    seq_t tmp = p3 * (seq_t)(n + 1);

    /* multiply by 4 and divide by 27 */
    tmp *= 4;
    tmp /= 27;

    return tmp;
}
