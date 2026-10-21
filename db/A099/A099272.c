/*
 * A099272  Unsigned member r=-14 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2) + 2*(-1)^(n+1) Initial values: a(0)=0,
 *            a(1)=1 Recurrence is valid for n>=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for consistency with idx_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t a_prev2 = 0;   /* a(0) */
    seq_t a_prev1 = 1;   /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t tmp = a_prev1 * (seq_t)16;   /* 16*a_prev1 */
        tmp = tmp - a_prev2;              /* - a_prev2 */
        if ((i & 1U) == 0)                /* even i → (-1)^(i+1) = -1 */
            tmp = tmp - 2;                /* + 2*(-1) */
        else                               /* odd i  → (+1) */
            tmp = tmp + 2;                /* + 2*1  */
        a_curr = tmp;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
