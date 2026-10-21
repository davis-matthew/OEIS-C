/*
 * A084339  7*n digit-reversed mod 7.
 * https://oeis.org/A084339
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = reverse_decimal(7*n) mod 7
 * Range:     u8 n <= 14, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes, for n >= 2, the value of
 * a(n) = (digits of 7*n reversed, interpreted as an integer) mod 7
 * The result is in the range 0..6. */
seq_t a(idx_t n)
{
    /* compute 7*n */
    seq_t m = (seq_t)n * 7;

    /* reverse the decimal digits of m */
    seq_t rev = 0;
    while (m > 0)
    {
        seq_t digit = m % 10;
        rev = rev * 10 + digit;
        m /= 10;
    }

    /* return rev modulo 7 */
    return rev % 7;
}
