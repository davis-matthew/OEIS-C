/*
 * A061908  Duplicate of A056020.
 * https://oeis.org/A061908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 9*(n/2) - 1 if n is even; * a(n) = 9*(n/2 + 1) - 8 if n
 *            is odd
 * Range:     i8 n <= 23, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = n / 2;          /* floor(n/2) */
    if ((n & 1) == 0) {
        /* even case: a = 9*(n/2) - 1 */
        return ((seq_t)9 * half) - 1;
    } else {
        /* odd case: a = 9*((n-1)/2 + 1) - 8 = 9*((n+1)/2) - 8 */
        return ((seq_t)9 * (half + 1)) - 8;
    }
}
