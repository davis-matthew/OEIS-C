/*
 * A017271  a(n) = (10*n)^3.
 * https://oeis.org/A017271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 162, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t res = n_s;      /* n */
    res *= n_s;           /* n^2 */
    res *= n_s;           /* n^3 */
    res *= (seq_t)10;     /* *10 */
    res *= (seq_t)10;     /* *10^2 */
    res *= (seq_t)10;     /* *10^3 = *1000 */
    return res;
}
