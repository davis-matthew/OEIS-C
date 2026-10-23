/*
 * A168283  a(n) = (10*n - 5*(-1)^n - 3)/4.
 * https://oeis.org/A168283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n - 5*(-1)^n - 3)/4
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)10 * (seq_t)n;
    if ((n & 1) == 0)           /* n is even */
        val -= (seq_t)5;
    else                        /* n is odd */
        val += (seq_t)5;
    val -= (seq_t)3;
    return val / (seq_t)4;
}
