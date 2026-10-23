/*
 * A126654  A 3 x 3 magic square.
 * https://oeis.org/A126654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = (A116198(n)+1)*4
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1 || n > 9)
        return 0;   /* values are defined only for n = 1..9 */

    /* Row‑major order of the 3×3 Lo Shu magic square, divided by 4:
       8  1  6
       3  5  7
       4  9  2
    */
    static const seq_t mag[3][3] = {{8, 1, 6},
                                    {3, 5, 7},
                                    {4, 9, 2}};
    idx_t r = (n - 1) / 3;
    idx_t c = (n - 1) % 3;
    return mag[r][c] * 4;
}
