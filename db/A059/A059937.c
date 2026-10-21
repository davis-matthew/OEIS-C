/*
 * A059937  Sum of binary numbers with n 1's and two (possibly leading) 0's.
 * https://oeis.org/A059937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+2) - 1)*n*(n+1)/2 Recurrence used: a(0)=0, a(1)=7,
 *            a(n) = a(n-1)*2*(n+1)/(n-1) + n*(n+1)/2 (n>=2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)7;

    seq_t a_prev = (seq_t)7;
    for (idx_t i = 2; i <= n; ++i)
    {
        /* temp = a_prev * 2 */
        seq_t temp = a_prev * ((seq_t)2);

        /* temp is divisible by (i-1) */
        seq_t div = temp / (seq_t)(i - 1);

        /* part from a_prev */
        seq_t part = div * (seq_t)(i + 1);

        /* add the additional term n*(n+1)/2 */
        seq_t add = ((seq_t)i) * ((seq_t)(i + 1)) / ((seq_t)2);

        a_prev = part + add;
    }
    return a_prev;
}
