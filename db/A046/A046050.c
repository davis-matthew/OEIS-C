/*
 * A046050  Sum of 19 but no fewer nonzero fourth powers.
 * https://oeis.org/A046050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 80*n - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) * (seq_t)80 - (seq_t)1;
}
