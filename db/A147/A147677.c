/*
 * A147677  Subtract 5, add 8, repeat.
 * https://oeis.org/A147677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(6*n + 9 - 13*(-1)^n)
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t num;
    if (n & 1)
    {
        /* odd n: 6*n + 9 + 13 = 6*n + 22 */
        num = (seq_t)6 * tn + (seq_t)22;
    }
    else
    {
        /* even n: 6*n + 9 - 13 = 6*n - 4 */
        num = (seq_t)6 * tn - (seq_t)4;
    }
    return num / 4;
}
