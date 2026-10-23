/*
 * A118402  Row sums of triangle A118401.
 * https://oeis.org/A118402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (5 + (-1)^n * (2*n - 3)) / 2 for n > 1; a(0) = 1, a(1) =
 *            1
 * Range:     i8 n <= 63, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(0) = a(1) = 1 */
        return (seq_t)1;

    seq_t N    = (seq_t)n;                     /* promote index to seq_t */
    seq_t sign = (n & 1U) ? (seq_t)-1 : (seq_t)1;   /* (-1)^n */
    seq_t num  = 5 + sign * ((2 * N) - 3);   /* numerator: always even */
    return num / 2;                          /* exact division by 2 */
}
