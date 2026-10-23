/*
 * A172486  Number of prime knots up to nine crossings with determinant 2n+1
 *          and signature 6.
 * https://oeis.org/A172486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of prime knots up to nine crossings with
 *            determinant 2n+1 and signature 6. The only n for which a(n) = 1
 *            are n = 1, 3, 9, 13, 15, 19. For all other n, a(n) = 0
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 1 || n == 3 || n == 9 || n == 13 || n == 15 || n == 19) ? 1 : 0;
}
