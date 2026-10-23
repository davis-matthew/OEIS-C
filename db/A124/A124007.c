/*
 * A124007  Number of permutations of n distinct letters (ABCD...) each of
 *          which appears thrice with n-3 fixed points.
 * https://oeis.org/A124007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*(n^3 - n)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 9 * (nn * nn * nn - nn);
}
