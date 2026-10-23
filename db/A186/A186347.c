/*
 * A186347  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=8i and g(j)=j^2. Complement of
 *          A186346.
 * https://oeis.org/A186347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n^2 + 1/2)/8) = n + floor((2*n^2 + 1)/16)
 * Range:     u8 n <= 11, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t nn   = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t num  = nn * 2 + 1;               /* 2*n^2 + 1 */
    seq_t add  = num / 16;                 /* floor division */
    return (seq_t)n + add;
}
