/*
 * A117378  Expansion of (1-4*x)/(1-x+x^2).
 * https://oeis.org/A117378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - a(n-2), a(0)=1, a(1)=-3
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = { (seq_t)1, (seq_t)-3, (seq_t)-4, (seq_t)-1, (seq_t)3, (seq_t)4 };
    return pattern[n % 6];
}
