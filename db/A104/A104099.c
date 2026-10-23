/*
 * A104099  a(n) = n * (10*n^2 - 6n + 1) = n * A087348(n).
 * https://oeis.org/A104099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(10*n^2 - 6*n + 1)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    seq_t ns = (seq_t)n;
    /* Compute the polynomial 10*n^2 - 6*n + 1 using seq_t arithmetic */
    seq_t inner = (seq_t)10 * ns * ns - (seq_t)6 * ns + (seq_t)1;
    /* Final result */
    return ns * inner;
}
