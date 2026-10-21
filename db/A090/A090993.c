/*
 * A090993  Number of meaningful differential operations of the n-th order on
 *          the space R^8.
 * https://oeis.org/A090993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 + 3*(-1)^n) * 3^floor((n-1)/2)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 77, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined offset, return 0 */

    /* compute 3^floor((n-1)/2) */
    idx_t exp = (n - 1) / 2;
    seq_t pow = 1;
    for (idx_t i = 0; i < exp; ++i) pow *= 3;

    /* multiplier (8 for odd n, 14 for even n) */
    seq_t mult = (n % 2 == 1) ? 8 : 14;

    return pow * mult;
}
