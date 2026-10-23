/*
 * A168232  a(n) = (2*n - 3*(-1)^n - 1)/2.
 * https://oeis.org/A168232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 3*(-1)^n - 1)/2
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t temp = (seq_t)2 * n - 1;
    if (n & 1U) {          /* n is odd: add 3 */
        temp += 3;
    } else {               /* n is even: subtract 3 */
        temp -= 3;
    }
    return temp / 2;
}
