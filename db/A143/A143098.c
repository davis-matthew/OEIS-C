/*
 * A143098  First differences of A143097.
 * https://oeis.org/A143098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=1; for n>1, a(n) = -1 if n≡0 (mod 3), otherwise 2
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;
    if ((n % (idx_t)3) == (idx_t)0)
        return (seq_t)(-1);
    return (seq_t)2;
}
