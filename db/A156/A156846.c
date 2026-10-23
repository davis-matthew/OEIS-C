/*
 * A156846  a(n) = 12167n - 3588.
 * https://oeis.org/A156846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12167*n - 3588
 * Range:     u8 none, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * (seq_t)12167) - (seq_t)3588;
}
