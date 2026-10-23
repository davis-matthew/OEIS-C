/*
 * A021788  Decimal expansion of 1/784.
 * https://oeis.org/A021788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 784) mod 10 (digit n of 1/784)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t remainder = 1;
    for (idx_t i = 0; i <= n; ++i)
    {
        remainder *= 10;          /* shift decimal point */
        seq_t digit = remainder / 784;
        remainder %= 784;        /* new remainder after digit extraction */
        if (i == n)
            return digit;         /* digit at desired index */
    }
    return 0;                       /* should never reach */
}
