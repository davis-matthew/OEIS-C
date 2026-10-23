/*
 * A181286  Partial sums of floor(n^2/3) (A000212).
 * https://oeis.org/A181286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 3*n)/18)
 * Range:     u8 n <= 12, u16 n <= 83, u32 n <= 1464, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (2*nn*nn*nn + 3*nn*nn - 3*nn) / (seq_t)18;
    return term;
}
