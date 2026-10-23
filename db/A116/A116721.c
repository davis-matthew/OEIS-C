/*
 * A116721  Number of permutations of length n which avoid the patterns 123,
 *          3142, 4312; or avoid the patterns 123, 3421, 4231.
 * https://oeis.org/A116721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 3*n^2 - 16*n + 24)/6 for n >= 2 a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1U)          /* only the offset case */
        return 1;

    seq_t nn = (seq_t)n;   /* convert index first */

    /* Compute the numerator with unsigned arithmetic
       in an order that keeps all intermediate values positive: */
    seq_t num = nn * nn * nn;          /* n^3            */
    num += (seq_t)3 * nn * nn;         /* + 3 n^2        */
    num += (seq_t)24;                  /* + 24           */
    num -= (seq_t)16 * nn;             /* - 16 n         */

    return num / 6;                     /* divide by 6     */
}
