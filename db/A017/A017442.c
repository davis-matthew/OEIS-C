/*
 * A017442  a(n) = (11*n + 4)^6.
 * https://oeis.org/A017442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 147, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n + (seq_t)4;
    seq_t sq = base * base;          /* base^2 */
    seq_t fourth = sq * sq;          /* base^4 */
    return fourth * sq;              /* base^6 */
}
