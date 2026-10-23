/*
 * A128792  a(n) = n^3*6^n.
 * https://oeis.org/A128792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero index yields zero */
    if (n == 0)
        return 0;

    /* Convert n to seq_t */
    seq_t sn = n;
    seq_t n2 = sn * sn;           /* n^2   */
    seq_t n3 = n2 * sn;           /* n^3   */

    /* Compute 6^n */
    seq_t pow6 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow6 *= (seq_t)6;

    /* Result */
    return n3 * pow6;
}
