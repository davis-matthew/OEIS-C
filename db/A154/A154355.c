/*
 * A154355  a(n) = 25*n^2 - 36*n + 13.
 * https://oeis.org/A154355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 - 36*n + 13
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 25*n^2 + 13 first to avoid unsigned underflow */
    seq_t term = 25 * nn * nn + 13;
    return term - 36 * nn;
}
