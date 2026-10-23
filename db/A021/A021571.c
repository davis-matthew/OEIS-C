/*
 * A021571  Decimal expansion of 1/567.
 * https://oeis.org/A021571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021571: decimal digits of 1/567, starting after the decimal point.
   a(0) is the first digit after the decimal point, a(1) the second, etc. */
seq_t a(idx_t n)
{
    /* The remainder from the division 1 / 567.
       Initially the remainder is 1. */
    idx_t r = 1;
    seq_t digit = 0;

    /* We need to perform n+1 steps: after the first step we have a(0),
       after the second a(1), ... after the (n+1)-th step a(n). */
    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t rt = r * 10;          /* r times 10: 10 * remainder */
        digit = rt / 567;           /* next decimal digit */
        r     = rt % 567;           /* new remainder for next step */
    }

    return digit;
}
