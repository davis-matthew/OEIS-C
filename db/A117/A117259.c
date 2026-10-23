/*
 * A117259  Column 0 of triangle A117258.
 * https://oeis.org/A117259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [∏_{j=0..n-1} (1-2*j)]/n! * 4^(n*(n-1)/2)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* multiply by (1-2*(k-1)) and by 4^(k-1), then divide by k */
        seq_t factor = 1 - (seq_t)(2 * (k - 1));
        res *= factor;
        idx_t shift = 2 * (k - 1);
        res <<= shift;
        res /= (seq_t)k;
    }
    return res;
}
