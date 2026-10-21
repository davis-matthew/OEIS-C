/*
 * A204502  Numbers k such that floor(k^2 / 9) is a square.
 * https://oeis.org/A204502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 12 for n > 5 a(1) = 0, a(2) = 1, a(3) = 2, a(4) =
 *            3, a(5) = 4
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5) {
        return (seq_t)(n - 1);   /* 0,1,2,3,4 */
    }
    return ((seq_t)3 * n) - (seq_t)12;
}
