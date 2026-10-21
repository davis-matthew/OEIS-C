/*
 * A071374  0 iff n is of the form 4^a*(8k+7), otherwise 1.
 * https://oeis.org/A071374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is NOT of the form 4^a*(8k+7); otherwise 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    /* remove all factors of 4 from m */
    while (m != 0 && (m & 3) == 0)  /* m % 4 == 0 */
        m >>= 2;                    /* divide by 4 */

    /* now test m % 8 == 7 */
    return (seq_t)((m & 7) == 7 ? 0 : 1);
}
