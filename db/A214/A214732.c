/*
 * A214732  a(n) = 25*n^2 + 15*n + 1021.
 * https://oeis.org/A214732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 + 15*n + 1021
 * Range:     u8 none, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid unwanted truncation */
    seq_t nn = (seq_t)n;
    return 25 * nn * nn + 15 * nn + 1021;
}
