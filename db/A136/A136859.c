/*
 * A136859  Numbers k such that k and k^2 use only the digits 0, 1, 4 and 6.
 * https://oeis.org/A136859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0; For n>=2: a(n) = 10^((n-2)/2) * (1 if n even else 4).
 *            Equivalent recurrence: a(n)=10*a(n-2) for n>3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 61,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1U)
        return 0U;                     /* a(1) = 0 */

    /* For n >= 2, compute power of 10 */
    idx_t exponent = (n - 2U) / 2U;    /* floor((n-2)/2) */
    seq_t p = pow10(exponent);        /* 10^exponent */

    if ((n & 1U) == 0U)                /* even n */
        return p;                     /* base = 1 */
    else
        return p * 4U;                /* base = 4 */
}
