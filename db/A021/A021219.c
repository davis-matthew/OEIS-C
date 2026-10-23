/*
 * A021219  Decimal expansion of 1/215.
 * https://oeis.org/A021219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 215) mod 10 (the decimal digits of
 *            1/215)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        unsigned int temp = (unsigned int)rem * 10;   /* product < 2150, fits unsigned int */
        digit = (seq_t)(temp / 215);                  /* quotient digit */
        rem   = (seq_t)(temp % 215);                  /* new remainder */
    }
    return digit;
}
