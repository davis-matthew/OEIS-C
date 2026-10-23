/*
 * A178069  a(n) = 12345679 * A001651(n).
 * https://oeis.org/A178069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 12345679 * ( n + (n-1)/2 )
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    seq_t k = (seq_t)n + ((seq_t)n - 1) / 2;
    return k * (seq_t)12345679;
}
