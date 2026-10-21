/*
 * A069080  a(n) = (2n+1)*(2n+2)*(2n+6)*(2n+7).
 * https://oeis.org/A069080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2n+1)*(2n+2)*(2n+6)*(2n+7)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    seq_t sn = (seq_t)n;

    /* Compute the four factors */
    seq_t f1 = sn * 2 + 1; /* 2n + 1 */
    seq_t f2 = sn * 2 + 2; /* 2n + 2 */
    seq_t f3 = sn * 2 + 6; /* 2n + 6 */
    seq_t f4 = sn * 2 + 7; /* 2n + 7 */

    /* Return the product of the factors */
    return f1 * f2 * f3 * f4;
}
