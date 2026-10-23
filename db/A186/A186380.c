/*
 * A186380  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=4i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186379.
 * https://oeis.org/A186380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n^2 + n + 1) / 8)
 * Range:     u8 n <= 15, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once to ensure all arithmetic is in seq_t */
    seq_t x = (seq_t)n;
    /* compute (n^2 + n + 1) / 8 */
    seq_t t = x * x + x + 1;
    t /= 8;                 /* integer division truncates toward zero; gives floor */
    /* add n back */
    t += x;
    return t;
}
