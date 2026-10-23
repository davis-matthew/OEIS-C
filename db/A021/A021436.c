/*
 * A021436  Decimal expansion of 1/432.
 * https://oeis.org/A021436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of 1/432. Obtain the digits by
 *            repeated long division: r_0 = 1; for i >= 0: d_i = ⌊ r_i * 10 /
 *            432 ⌋; r_{i+1} = (r_i * 10) mod 432; a(n) = d_n
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem   = 1;   /* current remainder */
    seq_t digit = 0;   /* digit for the current step */

    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10;                /* bring down next digit */
        digit  = rem / (seq_t)432;  /* quotient is the decimal digit */
        rem   %= (seq_t)432;        /* new remainder for next step */
    }
    return digit;
}
