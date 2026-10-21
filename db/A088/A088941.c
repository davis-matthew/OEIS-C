/*
 * A088941  a(n)=12*sum(1<=i<=j<=k<=n,i*j/k).
 * https://oeis.org/A088941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/24)*(9*n^3 + 58*n^2 + 123*n + 98)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type first to avoid narrowing */
    seq_t nn = (seq_t)n;
    /* Compute n^2 and n^3 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    /* Evaluate the polynomial 9*n^3 + 58*n^2 + 123*n + 98 */
    seq_t poly = 9 * n3 + 58 * n2 + 123 * nn + 98;
    /* Multiply by n and divide by 24 – the division is exact by definition */
    seq_t num = nn * poly;
    return num / 24;
}
