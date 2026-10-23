/*
 * A021880  Decimal expansion of 1/876.
 * https://oeis.org/A021880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 876) mod 10 (decimal digits of 1/876)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Remainder stays below 876; store it in a native integer type. */
    idx_t r = 1;                     /* start with the numerator 1 */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10ULL;                   /* bring next 0 into the fraction */
        seq_t d = (seq_t)(r / 876ULL);/* current decimal digit          */
        r  %= 876ULL;                 /* new remainder                  */
        if (i == n)
            return d;
    }
    return (seq_t)0;                  /* unreachable; keeps compiler happy */
}
