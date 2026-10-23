/*
 * A134667  Period 6: repeat [0, 1, 0, 0, 0, -1].
 * https://oeis.org/A134667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = { 1 if n mod 6 = 1, * -1 if n mod 6 = 5, * 0 otherwise }
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    if (r == 1u)
        return (seq_t)1;
    if (r == 5u)
        return (seq_t)-1;
    return (seq_t)0;
}
