/*
 * A063210  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          42 ).
 * https://oeis.org/A063210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n) / floor(n/2) for n ≥ 2, a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;          /* a(1) = 1 (a(0) is outside the offset) */

    seq_t nn = (seq_t)n;                 /* convert index to the arithmetic type */
    seq_t numerator = nn * nn - nn;      /* n^2 - n */
    seq_t denom = nn / 2;                /* floor(n/2), integer division on unsigned integers */
    return numerator / denom;
}
