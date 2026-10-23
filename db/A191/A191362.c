/*
 * A191362  Number of the diagonal of the dispersion of the even positive
 *          integers that contains n.
 * https://oeis.org/A191362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((m-1)/2) - r where n = 2^r * m, m odd, r ≥ 0
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count trailing zeros (v2(n)) */
    idx_t r = 0;
    while ((n & 1) == 0)
    {
        n >>= 1;
        r++;
    }

    /* n now holds the odd part m */
    seq_t m = (seq_t)n;
    seq_t result = (m - 1) / 2 - (seq_t)r;   /* all ops in seq_t */
    return result;
}
