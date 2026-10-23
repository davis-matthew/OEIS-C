/*
 * A141631  a(n) = 3*n^2 - 4*n + 3.
 * https://oeis.org/A141631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 3*n^2 - 4*n + 3
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, return 0 for n == 0 (offset is 1). */
    if (n == 0) return 0;
    seq_t x = (seq_t)n;
    return x * ((x * 3) - 4) + 3;
}
