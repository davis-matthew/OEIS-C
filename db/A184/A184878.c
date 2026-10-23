/*
 * A184878  Third column (negated) of triangle in A182971.
 * https://oeis.org/A184878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            n*(n-1)*(n-2)*(n-3)*(n-4)*(n+2)*(n+1)*(35*n^2+182*n+240)/45360
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 8, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 5 the product contains a zero term → a(n) = 0 */
    if (n < 5)
        return 0;

    seq_t sn = (seq_t)n;

    /* Compute product of the seven linear factors */
    seq_t prod = sn;
    prod = prod * (sn - 1);
    prod = prod * (sn - 2);
    prod = prod * (sn - 3);
    prod = prod * (sn - 4);
    prod = prod * (sn + 2);
    prod = prod * (sn + 1);

    /* Compute the quadratic polynomial 35*n^2 + 182*n + 240 */
    seq_t poly = (seq_t)35 * sn * sn + (seq_t)182 * sn + (seq_t)240;

    /* Combine and divide by 45360 */
    seq_t numerator = prod * poly;
    return numerator / (seq_t)45360;
}
