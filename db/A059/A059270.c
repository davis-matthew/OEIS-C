/*
 * A059270  a(n) is both the sum of n+1 consecutive integers and the sum of
 *          the n immediately higher consecutive integers.
 * https://oeis.org/A059270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);   /* n*(n+1)    */
    prod /= 2;                    /* divide by 2   */
    prod *= (2 * nn + 1);         /* * (2*n+1)    */
    return prod;
}
