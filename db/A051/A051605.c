/*
 * A051605  a(n) = (3*n+5)!!!/5!!!.
 * https://oeis.org/A051605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n+5)*a(n-1), a(0) = 1 Recurrence follows from a(n) =
 *            ((3*n+5)(!^3))/5(!^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) /* multiply only the factors 8, 11, 14, …   */
        res *= (seq_t)(3 * i + 5);
    return res;
}
