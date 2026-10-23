/*
 * A100183  Structured hexagonal anti-prism numbers.
 * https://oeis.org/A100183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (13*n^3 - 9*n^2 + 2*n) / 6
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 763, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;        /* n^2 */
    seq_t nn3 = nn2 * nn;       /* n^3 */
    seq_t val = (13 * nn3 - 9 * nn2 + 2 * nn) / 6;
    return val;
}
