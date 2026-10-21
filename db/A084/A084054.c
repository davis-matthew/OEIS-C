/*
 * A084054  5*n digit-reversed mod 5.
 * https://oeis.org/A084054
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (first digit of 5*n) mod 5 (where the first digit is
 *            taken in decimal representation)
 * Range:     u8 n <= 51, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 2) the value is arbitrary */
    if (n < 2)
        return (seq_t)0;

    /* Compute the product 5*n using seq_t arithmetic */
    seq_t prod = (seq_t)n * (seq_t)5;

    /* Determine the first decimal digit of prod */
    seq_t first = prod;
    while (first >= (seq_t)10)
        first /= (seq_t)10;

    /* Return the first digit modulo 5 */
    return first % (seq_t)5;
}
