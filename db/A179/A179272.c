/*
 * A179272  Sharp upper bound on Rosgen overlap number n-vertex graph with n
 *          >= 14, formula abused here for nonnegative integers.
 * https://oeis.org/A179272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n^2/4 - n/2 - 1)
 * Range:     i8 n <= 12, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t num = nn * nn - 2 * nn - 4;        /* numerator of (n^2 - 2n - 4)/4 */
    seq_t div = num / 4;                     /* truncating division */

    /* adjust for floor division when numerator is negative */
    if (num < 0 && (num % 4 != 0))
        div -= 1;

    return div;
}
