/*
 * A228406  Sequences from the quartic oscillator.
 * https://oeis.org/A228406
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(4+44*n/5+16*n^2/5)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(-1) is defined as 0 */
    if (n == -1)
        return (seq_t)0;

    /* cast index to the sequence type to avoid intermediate overflows */
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + 1;
    seq_t sn2 = sn + 2;
    seq_t sn3 = sn + 3;

    /* product of the three consecutive numbers */
    seq_t prod3 = sn1 * sn2 * sn3;

    /* numerator of the rational part: 20 + 44*n + 16*n^2 */
    seq_t numerator = (seq_t)20 + ((seq_t)44 * sn) + ((seq_t)16 * sn * sn);

    /* divide the product by 5 before the final multiplication to avoid overflow.
       Exactly one of prod3 or numerator is divisible by 5. */
    if ((prod3 % (seq_t)5) == (seq_t)0) {
        prod3 /= (seq_t)5;
    } else {
        numerator /= (seq_t)5;
    }

    /* final result */
    return prod3 * numerator;
}
