/*
 * A160912  [1, 3, 5, 7, ...] convolved with [1, 4, 0, 0, 0, ...].
 * https://oeis.org/A160912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 1; a(n) = 10*n - 13 for n >= 2
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    return ((seq_t)10 * (seq_t)n) - (seq_t)13;
}
