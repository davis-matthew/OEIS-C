/*
 * A201618  Number of n X 1 0..3 arrays with rows and columns
 *          lexicographically nondecreasing and every element equal to at
 *          least one horizontal or vertical neighbor.
 * https://oeis.org/A201618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)*(n-2)*(n-3))/6 + 4*(n-2) for n > 4 a(1) = 0, a(2)
 *            = 4, a(3) = 4, a(4) = 10
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)          return (seq_t)0;
    if (n == 2 || n == 3) return (seq_t)4;
    if (n == 4)          return (seq_t)10;

    /* n >= 5 */
    seq_t tn   = (seq_t)n;
    seq_t prod = (tn - 1) * (tn - 2);
    prod       = prod * (tn - 3);
    seq_t term1 = prod / (seq_t)6;      /* (n-1)(n-2)(n-3)/6 */
    seq_t term2 = (seq_t)4 * (tn - 2);   /* 4*(n-2)   */
    return term1 + term2;
}
