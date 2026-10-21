/*
 * A094793  a(n) = (1/n!)*A001688(n).
 * https://oeis.org/A094793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 6*n^3 + 17*n^2 + 20*n + 9
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;        /* real index as seq_t */
    seq_t res = x + (seq_t)6;    /* n + 6 */
    res = res * x + (seq_t)17;   /* (n + 6)*n + 17 */
    res = res * x + (seq_t)20;   /* ((n + 6)*n + 17)*n + 20 */
    res = res * x + (seq_t)9;    /* (((n + 6)*n + 17)*n + 20)*n + 9 */
    return res;
}
