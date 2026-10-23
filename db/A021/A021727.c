/*
 * A021727  Decimal expansion of 1/723.
 * https://oeis.org/A021727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 723) mod 10 (equivalently, n-th decimal
 *            digit of 1/723 after the point)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1;       /* start remainder with integer part of 1/723 */
    seq_t digit = 0;     /* current decimal digit */

    /* Perform long–division to obtain the n-th digit */
    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * ((seq_t)10);       /* shift remainder left by one decimal place */
        digit = rem / ((seq_t)723);      /* next digit */
        rem   = rem % ((seq_t)723);      /* new remainder for following digits */
    }

    return digit;
}
