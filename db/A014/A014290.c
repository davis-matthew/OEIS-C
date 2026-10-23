/*
 * A014290  Submitted to the OEIS as "Seen on a billboard in England,
 *          definition unknown".
 * https://oeis.org/A014290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n² (the squares), but with the observed billboard
 *            distortions: a(1) = 2 and a(5) = 15
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 5, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 5)
        return (seq_t)15;
    seq_t m = (seq_t)n;
    return m * m;
}
