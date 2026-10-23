/*
 * A143839  Ulam's spiral (SSE spoke).
 * https://oeis.org/A143839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 25*n + 10
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t term = (seq_t)16 * tn * tn + (seq_t)10;
    return term - (seq_t)25 * tn;
}
