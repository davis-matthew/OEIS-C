/*
 * A021607  Decimal expansion of 1/603.
 * https://oeis.org/A021607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = digit number n of the decimal expansion of 1/603
   (offset 0: a(0) is the first digit after the decimal point) */
seq_t a(idx_t n)
{
    seq_t digit = 0;
    seq_t rem   = 1;          /* initial remainder 1 / 603 */

    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10;          /* shift decimal point right */
        digit  = rem / 603;   /* next decimal digit */
        rem   %= 603;         /* new remainder for next step */
    }
    return digit;
}
