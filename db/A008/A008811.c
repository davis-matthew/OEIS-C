/*
 * A008811  Expansion of x*(1+x^4)/((1-x)^2*(1-x^4)).
 * https://oeis.org/A008811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/4) + (n mod 4 > 0)
 * Range:     u8 n <= 15, u16 n <= 398, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 4;                          /* n mod 4 */
    seq_t sq = (seq_t)n * (seq_t)n;           /* n^2 in seq_t */
    seq_t res = sq / 4;                       /* floor division by 4 */
    if (r > 0) ++res;                         /* add 1 if n mod 4 > 0 */
    return res;
}
