/*
 * A118824  2-adic continued fraction of zero, where a(n) = -2 if n is odd,
 *          A006519(n/2) otherwise.
 * https://oeis.org/A118824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, sign
 * Formula:   a(n) = -2 if n is odd, otherwise A006519(n/2) where A006519(k)
 *            is the largest power of 2 dividing k
 * Range:     i8 n <= 255, i16 n <= 9962, i32 n <= 9962, i64 n <= 9962,
 *            i128 n <= 9962, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return -2 for odd indices */
    if ((n & 1) != 0)
        return (seq_t)-2;

    /* n is even: compute n/2 */
    idx_t m = n >> 1;          /* n/2 */
    seq_t result = 1;          /* will hold the highest power of 2 dividing m */

    /* shift right while m is even, multiply result by 2 each time */
    while ((m & 1) == 0) {
        result <<= 1;          /* multiply by 2 */
        m >>= 1;               /* divide m by 2 */
    }

    return result;
}
