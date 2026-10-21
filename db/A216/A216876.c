/*
 * A216876  20k^2-20k-5 interleaved with 20k^2+5 for k=>0.
 * https://oeis.org/A216876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5 * (n*(n-2) - 1) for even n; * a(n) = 5 * (n*(n-2) + 2)
 *            for odd n
 * Range:     i8 n <= 6, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0)
        return 5 * (N * (N - 2) - 1);
    else
        return 5 * (N * (N - 2) + 2);
}
