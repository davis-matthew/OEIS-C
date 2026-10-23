/*
 * A021517  Decimal expansion of 1/513.
 * https://oeis.org/A021517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 513) mod 10 (decimal digit of 1/513)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = 1;        /* current remainder of dividing 1 by 513 */
    seq_t digit = 0;      /* result digit for position n */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t mult = rem * 10;          /* multiply by 10 for next digit */
        digit = (seq_t)(mult / 513);    /* integer part is the digit */
        rem   =  mult % 513;             /* new remainder */
    }

    return digit;
}
