/*
 * A021986  Decimal expansion of 1/982.
 * https://oeis.org/A021986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊10^(n+1)/982⌋ mod 10 (decimal digit of 1/982 after the
 *            dot)
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* remainder of the current fraction 1/982 * 10^k, 0 ≤ rem < 982 */
    seq_t rem   = 1;
    seq_t digit = 0;

    /* iterate through the digits up to the requested index n */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem   *= 10;                         /* shift next decimal place */
        digit  = rem / (seq_t)982;           /* current digit          */
        rem   %= (seq_t)982;                 /* update remainder       */
    }

    return digit;
}
