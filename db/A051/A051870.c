/*
 * A051870  18-gonal (or octadecagonal) numbers: a(n) = n*(8*n-7).
 * https://oeis.org/A051870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (8*n - 7)
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 8*n in seq_t to avoid mixing with idx_t */
    seq_t sn = (seq_t)n;
    seq_t eight_n = (seq_t)8 * sn;
    seq_t term = eight_n - (seq_t)7;
    return sn * term;
}
