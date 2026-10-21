/*
 * A084055  6*n digit-reversed mod 6.
 * https://oeis.org/A084055
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = rev(6*n) mod 6 where rev(x) is the decimal digit reverse
 *            of x (leading zeros discarded)
 * Range:     u8 n <= 18, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 6*n.  Use the supplied seq_t type for arithmetic. */
    seq_t m = (seq_t)n * 6;

    /* Reverse the decimal digits of m. */
    seq_t rev = 0;
    while (m > 0) {
        rev = rev * 10 + (m % 10);
        m = m / 10;
    }

    /* Return the result modulo 6. */
    return rev % 6;
}
