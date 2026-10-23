/*
 * A134990  Interleave two arithmetic progressions 8,10,12,14,... and
 *          15,13,11,9,... of differences +2 and -2 respectively.
 * https://oeis.org/A134990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 8 + 2*((n+1)/2 - 1) for odd n a(n) = 15 - 2*(n/2 - 1) for
 *            even n
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; undefined for n < 1 */
    if ((n & 1) == 1) {                       /* odd n */
        idx_t k = (n + 1) / 2;                /* k = (n+1)/2 */
        return (seq_t)8 + ((seq_t)2 * ((seq_t)k - 1));
    } else {                                   /* even n */
        idx_t k = n / 2;                      /* k = n/2 */
        return (seq_t)15 - ((seq_t)2 * ((seq_t)k - 1));
    }
}
