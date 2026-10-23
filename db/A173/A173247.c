/*
 * A173247  a(0) = -1 and a(n) = (-1)^(n+1)*(3*n^2 - n + 4)/2 for n >= 1.
 * https://oeis.org/A173247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1 ; for n ≥ 1, a(n) = (-1)^(n+1)*(3*n^2 - n + 4)/2
 * Range:     i8 n <= 6, i16 n <= 106, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    seq_t nn = (seq_t)n;
    seq_t val = 3*nn*nn - nn + 4;   /* numerator 3*n^2 - n + 4 */
    val /= 2;                       /* integer division by 2 */
    if ((n & 1) == 0) val = -val;   /* n even → negative sign, else positive */
    return val;
}
