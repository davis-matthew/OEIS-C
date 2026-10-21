/*
 * A051871  19-gonal (or enneadecagonal) numbers: n(17n-15)/2.
 * https://oeis.org/A051871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (17*n - 15) / 2
 * Range:     u8 n <= 5, u16 n <= 88, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t tmp   = ((seq_t)17 * n_seq) - (seq_t)15;
    return n_seq * tmp / 2;
}
