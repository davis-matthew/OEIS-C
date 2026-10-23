/*
 * A021489  Decimal expansion of 1/485.
 * https://oeis.org/A021489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point of the decimal
 *            expansion of 1/485
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int rem = 1;          /* remainder of the long division (mod 485) */
    seq_t digit = 0;
    /* compute digits from 0 up to n */
    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)(rem * 10) / 485;
        rem   = (rem * 10) % 485;
    }
    return digit;
}
