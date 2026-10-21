/*
 * A216852  18k^2-36k+9 interleaved with 18k^2-18k+9 for k>=0.
 * https://oeis.org/A216852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (9/4)*(2*n*(n-4)-3*(-1)^n+7)
 * Range:     i8 n <= 7, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long N = (long long)n;
    int sign = (n & 1) ? -1 : 1;
    long long t = 2 * N * (N - 4) - 3 * sign + 7;
    return (seq_t)(9 * t / 4);
}
