/*
 * A051396  a(n) = (2*n-2)*(2*n-3)*a(n-1)+1.
 * https://oeis.org/A051396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = (2*n-2)*(2*n-3)*a(n-1)+1 for n>=2 (OEIS
 *            A051396)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Base case a(1) */
    seq_t res = (seq_t)1;
    if (n == 1)
        return res;

    /* Iterate using the recurrence relation */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t f1 = (seq_t)(2 * i - 2);
        seq_t f2 = (seq_t)(2 * i - 3);
        res = f1 * f2 * res + (seq_t)1;
    }
    return res;
}
