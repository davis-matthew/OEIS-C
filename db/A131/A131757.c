/*
 * A131757  Period 10: repeat 3, 3, 3, -7, 3, 3, -7, 3, 3, -7.
 * https://oeis.org/A131757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3 for n mod 10 ∈ {0,1,2,4,5,7,8}, otherwise -7
 * Range:     i8 n <= 110, i16 n <= 110, i32 n <= 110, i64 n <= 110,
 *            i128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 10;
    if (r == 3 || r == 6 || r == 9)
        return (seq_t)-7;
    return (seq_t)3;
}
