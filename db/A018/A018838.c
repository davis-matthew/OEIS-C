/*
 * A018838  Minimum number of steps for a knight to reach (n,n) on an infinite
 *          chessboard.
 * https://oeis.org/A018838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*ceil(n/3) for n >= 3; a(0) = 0; a(1) = 2; a(2) = 4
 * Range:     u8 n <= 381, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 2;
    if (n == 2)
        return 4;

    /* ceil(n/3) = n/3 with an increment if n%3 != 0 */
    idx_t ceil_n3 = n / 3;
    if (n % 3)
        ++ceil_n3;

    /* Result is 2 * ceil_n3 */
    return (seq_t)(2 * ceil_n3);
}
