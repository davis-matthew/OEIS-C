/*
 * A134633  5*n^5 + 3*n^3 + 2*n^2. Coefficients and exponents are the prime
 *          numbers in decreasing order.
 * https://oeis.org/A134633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^5 + 3*n^3 + 2*n^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;             /* base value n as seq_t          */
    seq_t n2 = sn * sn;              /* n^2                              */
    seq_t n3 = n2 * sn;              /* n^3                              */
    seq_t n5 = n3 * n2;              /* n^5 = n^3 * n^2                  */
    return 5 * n5 + 3 * n3 + 2 * n2; /* final formula                   */
}
