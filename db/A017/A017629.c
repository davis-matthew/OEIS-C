/*
 * A017629  a(n) = 12*n + 9.
 * https://oeis.org/A017629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n + 9
 * Range:     u8 n <= 20, u16 n <= 5460, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)12 * (seq_t)n) + (seq_t)9;
}
