/*
 * A024397  a(n) = s(1)*s(2)*...*s(n+1)*(1/s(2) - 1/s(3) + ... + c/s(n+1)),
 *          where c = (-1)^(n+1) and s(k) = 3k-1 for k = 1,2,3,...
 * https://oeis.org/A024397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(1)*s(2)*...*s(n+1) * (1/s(2) - 1/s(3) + ... +
 *            (-1)^(n+1)/s(n+1)), where s(k) = 3*k - 1. The sequence has
 *            offset 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n = 1 the value is 2. */
    if (n == 1)
        return (seq_t)2;

    /* Start with a(1) = 2 and P(1) = s(1)*s(2) = 2*5 = 10. */
    seq_t a_val = (seq_t)2;
    seq_t P_val = (seq_t)10;

    for (idx_t i = 2; i <= n; ++i) {
        /* s(i+1) = 3*(i+1)-1 */
        seq_t sn = (seq_t)(3 * (i + 1) - 1);

        seq_t temp = a_val * sn;
        int sign = ((i + 1) & 1) ? -1 : 1;

        if (sign == 1)
            a_val = temp + P_val;
        else
            a_val = temp - P_val;  /* a_val > P_val for all n >= 2 */

        /* Update the product for the next iteration. */
        P_val = P_val * sn;
    }

    return a_val;
}
