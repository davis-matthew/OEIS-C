/*
 * A021695  Decimal expansion of 1/691.
 * https://oeis.org/A021695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/691 (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem   = 1;   /* remainder before the first division */
    seq_t     digit = 0;  /* will hold the digit at position n */

    /* Perform long division 10 times up to the nth digit (inclusive) */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem = rem * 10ULL;                 /* bring down the next decimal place */
        digit = (seq_t)(rem / 691ULL);      /* integer quotient gives the digit   */
        rem = rem % 691ULL;                 /* remainder for the next step         */
    }

    return digit;
}
