/*
 * A084340  8*n digit-reversed mod 8.
 * https://oeis.org/A084340
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = reverse(8*n) mod 8, with a(2) the first term
 * Range:     u8 n <= 31, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 8*n as a seq_t value. */
    seq_t x = (seq_t)n * 8;

    /* Accumulator for the reversed number modulo 8. */
    seq_t y = 0;

    /* Reverse the decimal digits of x while taking everything modulo 8.
       Since 10 ≡ 2 (mod 8), the recurrence for y is
           y_{new} ≡ y_{old}*2 + digit   (mod 8). */
    while (x != 0)
    {
        seq_t digit = x % 10;   /* Extract least significant decimal digit. */
        y = (y * 2 + digit) % 8;/* Update reversed value modulo 8. */
        x /= 10;                /* Remove the processed digit. */
    }

    return y;
}
