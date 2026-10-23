/*
 * A105163  a(n) = (n^3 - 7*n + 12)/6.
 * https://oeis.org/A105163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 7*n + 12)/6, offset 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first */
    seq_t nn = (seq_t)n;
    /* Compute numerator: n^3 - 7*n + 12 */
    seq_t num = nn * nn * nn - (seq_t)7 * nn + (seq_t)12;
    /* Divide by 6 (division is exact by definition) */
    return num / (seq_t)6;
}
