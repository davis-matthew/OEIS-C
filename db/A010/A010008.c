/*
 * A010008  a(0) = 1, a(n) = 18*n^2 + 2 for n>0.
 * https://oeis.org/A010008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 18*n^2 + 2 for n > 0
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* Compute 18*n*n + 2 using seq_t arithmetic */
    seq_t tn = (seq_t)n;
    seq_t square = tn * tn;     /* n^2   */
    seq_t result = (seq_t)18 * square + (seq_t)2;
    return result;
}
