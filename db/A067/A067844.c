/*
 * A067844  Numbers k such that k and 2^k end with the same digit.
 * https://oeis.org/A067844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*n - 4*(-1)^n Equivalent: a(n) = 10*n + 4 (n odd) a(n)
 *            = 10*n - 4 (n even)
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n * 10;
    if (n & 1)               /* n is odd */
        res += 4;
    else                     /* n is even */
        res -= 4;
    return res;
}
