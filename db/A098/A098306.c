/*
 * A098306  Unsigned member r=-6 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A098306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (T(n,4) - (-1)^n)/5 Recurrence used: a(0)=0, a(1)=1 a(n)
 *            = 8*a(n-1) - a(n-2) + 2*(-1)^(n+1) (n>=2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)0;   /* a(n-2)  */
    seq_t a1 = (seq_t)1;   /* a(n-1)  */
    seq_t temp;

    for (idx_t i = 2; i <= n; ++i) {
        /* temp = 8*a1 - a0 */
        temp = a1 << 3;         /* 8*a1 */
        temp -= a0;             /* 8*a1 - a0 */

        /* add or subtract 2 depending on parity of i */
        if (i & 1)               /* i is odd -> +2   */
            temp += 2;
        else                     /* i is even -> -2  */
            temp -= 2;

        a0 = a1;                 /* shift for next iteration */
        a1 = temp;
    }

    return a1;
}
