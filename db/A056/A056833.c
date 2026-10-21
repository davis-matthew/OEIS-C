/*
 * A056833  Nearest integer to n^2/7.
 * https://oeis.org/A056833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n^2 / 7 (i.e., round(n^2 / 7))
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;       /* cast index to seq_t for arithmetic   */
    seq_t t = x * x;          /* n^2 in seq_t                           */
    seq_t q = t / 7;          /* integer division part                 */
    seq_t r = t % 7;          /* remainder part                         */
    /* rounding: add 1 if fractional part >= 0.5  <=> r >= 4   */
    if (r >= 4) q++;
    return q;
}
