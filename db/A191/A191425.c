/*
 * A191425  Among all real (0,1) n X n matrices such that |det A| = permanent
 *          A, the maximal value of |det A|.
 * https://oeis.org/A191425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 3, a(5) = 5, a(6) = 8, a(n)
 *            = 24 for n >= 7
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return 1;
    if (n == 3)          return 2;
    if (n == 4)          return 3;
    if (n == 5)          return 5;
    if (n == 6)          return 8;
    return 24;
}
