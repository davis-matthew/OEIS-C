/*
 * A213526  a(n) = 3*n AND n, where AND is the bitwise AND operator.
 * https://oeis.org/A213526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = (3 * n) & n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3 * n in seq_t, then bitwise AND with n. */
    return ((seq_t)n * (seq_t)3) & (seq_t)n;
}
