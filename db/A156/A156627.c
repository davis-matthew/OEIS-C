/*
 * A156627  a(n) = 4394*n - 1820.
 * https://oeis.org/A156627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4394*n - 1820
 * Range:     u8 none, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)4394 * (seq_t)n) - (seq_t)1820;
}
