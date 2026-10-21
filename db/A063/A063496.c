/*
 * A063496  a(n) = (2*n - 1)*(8*n^2 - 8*n + 3)/3.
 * https://oeis.org/A063496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1)*(8*n^2 - 8*n + 3)/3
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 662, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  The offset is 1; if called with n == 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t sn = (seq_t)n;

    /* term1 = 2*n - 1 */
    seq_t term1 = (sn << 1) - (seq_t)1;

    /* term2 = 8*n^2 - 8*n + 3 = (sn*sn<<3) - (sn<<3) + 3 */
    seq_t term2 = (sn * sn) << 3;
    term2 -= sn << 3;
    term2 += (seq_t)3;

    /* product = term1 * term2 */
    seq_t product = term1 * term2;

    /* divide by 3 (exact integer division) */
    return product / (seq_t)3;
}
