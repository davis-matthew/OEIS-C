/*
 * A021681  Decimal expansion of 1/677.
 * https://oeis.org/A021681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/677⌋ mod 10 – the n‑th digit after the decimal
 *            point of 1/677
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t rem = 1;   /* remainder before extracting the next digit */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        digit = (rem * (seq_t)10) / (seq_t)677;
        rem   = (rem * (seq_t)10) % (seq_t)677;
    }
    return digit;
}
