/*
 * A090168  Floor( 3n/2 ) - floor( 2n/3 ).
 * https://oeis.org/A090168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊3n/2⌋ − ⌊2n/3⌋
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute ⌊3n/2⌋ as n + ⌊n/2⌋  */
    seq_t floor3 = (seq_t)n + (seq_t)(n / 2);

    /* compute ⌊2n/3⌋ using quotient and remainder */
    idx_t q = n / 3;
    idx_t r = n % 3;
    seq_t floor2 = (seq_t)(2 * q) + ((r == 2) ? 1 : 0);

    return floor3 - floor2;
}
