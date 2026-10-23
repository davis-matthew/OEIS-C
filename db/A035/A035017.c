/*
 * A035017  One quarter of 9-factorial numbers.
 * https://oeis.org/A035017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{j=1..n} (9*j-5) Recurrence: a(1)=1, a(n) =
 *            (9*n-5)*a(n-1) for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n == 0)
        return (seq_t)0;   /* undefined, return 0 as a safe default */

    seq_t result = (seq_t)1;    /* a(1) */
    for (idx_t j = 2; j <= n; ++j)
        result *= (seq_t)(9 * j - 5);   /* recurrence step */

    return result;
}
