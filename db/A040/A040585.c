/*
 * A040585  Continued fraction for sqrt(610).
 * https://oeis.org/A040585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24; for n>0, a(n)=pattern[(n-1) mod 9] where pattern = {1,
 *            2, 3, 5, 5, 3, 2, 1, 48}
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 24;
    static const seq_t pattern[9] = {1, 2, 3, 5, 5, 3, 2, 1, 48};
    return pattern[(n - 1) % 9];
}
