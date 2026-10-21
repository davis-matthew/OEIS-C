/*
 * A228012  The 2-color Rado number for the equation x_1 + x_2 + ... + x_n =
 *          2*x_0.
 * https://oeis.org/A228012
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * For n = 2, a(n) = 1; * For n = 3, a(n) = 4; * For n = 4, a(n)
 *            = 5; * For n >= 5, a(n) = ceil( ceil(n/2) * n / 2 )
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the explicitly defined initial values. */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)5;

    /* n >= 5 */
    /* ceil(n/2) = (n + 1) / 2 for integer n. */
    seq_t half = (seq_t)((n + 1) / 2);
    seq_t prod = half * (seq_t)n;        /* product = ceil(n/2) * n  */
    /* ceil(prod / 2) = (prod + 1) / 2 for integer prod. */
    return (prod + 1) / 2;
}
