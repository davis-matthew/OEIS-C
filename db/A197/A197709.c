/*
 * A197709  Floor((n+1/n)^8).
 * https://oeis.org/A197709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + 1/n)^8)
 * Range:     u8 none, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t s_n = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = s_n * s_n;         /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n6 = n4 * n2;           /* n^6 */
    seq_t n8 = n4 * n4;           /* n^8 */

    /* Integer part from the binomial expansion */
    seq_t sumPoly = n8
        + ((seq_t)8 * n6)
        + ((seq_t)28 * n4)
        + ((seq_t)56 * n2)
        + ((seq_t)70);

    /* Remainder part: floor(56/n^2 + 28/n^4 + 8/n^6 + 1/n^8) */
    seq_t numerator2 = ((seq_t)56 * n6)
        + ((seq_t)28 * n4)
        + ((seq_t)8 * n2)
        + ((seq_t)1);
    seq_t fractionFloor = numerator2 / n8;

    return sumPoly + fractionFloor;
}
