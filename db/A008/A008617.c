/*
 * A008617  Expansion of 1/((1-x^2)(1-x^7)).
 * https://oeis.org/A008617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n + 21 + 7*(-1)^n)/28)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t num;

    /* For even n add 7, for odd n subtract 7 from 21 */
    if ((n & 1) == 0)         /* n is even */
        num = 2 * s + 28;     /* 21 + 7 */
    else                      /* n is odd */
        num = 2 * s + 14;     /* 21 - 7 */

    return num / 28;          /* integer division truncates toward 0 */
}
