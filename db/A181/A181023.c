/*
 * A181023  Maximum number of 1s in an n X n binary matrix with no four 1s
 *          adjacent in a line along a row, column or diagonally.
 * https://oeis.org/A181023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2 for n <= 3; otherwise a(n) = floor(3*n^2 / 4) if n
 *            mod 4 ≠ 2, and a(n) = floor(3*n^2 / 4) – 1 when n mod 4 = 2
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        seq_t nn = (seq_t)n;
        return nn * nn;
    }
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;       /* n^2 */
    val *= 3;                  /* 3*n^2 */
    val /= 4;                  /* floor(3*n^2 / 4) */
    if ((n & 3) == 2) {        /* n mod 4 == 2 */
        val -= 1;
    }
    return val;
}
