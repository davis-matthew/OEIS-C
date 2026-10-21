/*
 * A060783  Number of conics which pass through 3 points and are bitangent to
 *          a general curve of order n.
 * https://oeis.org/A060783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*n*(n^2 + 3*n - 6)/2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t to avoid overflow on intermediate calculations */
    seq_t nn = (seq_t)n;
    /* Compute (n-1)*n / 2 first – guaranteed to be integer */
    seq_t term1 = nn * (nn - 1);
    term1 /= 2;
    /* Compute the quadratic part */
    seq_t term2 = nn * nn + 3 * nn - 6;
    /* Final result */
    return term1 * term2;
}
