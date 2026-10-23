/*
 * A021583  Decimal expansion of 1/579.
 * https://oeis.org/A021583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/579 (digits after the decimal
 *            point)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the nth digit after the decimal point of 1/579.
       Uses the long–division method: start with remainder 1,
       repeatedly multiply by 10, divide by 579 to get the digit,
       and take the new remainder. */
    seq_t rem = 1;          /* current remainder (0 <= rem < 579) */
    seq_t digit = 0;        /* holds the last computed digit */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        digit = rem / 579;
        rem   = rem % 579;
    }
    return digit;
}
