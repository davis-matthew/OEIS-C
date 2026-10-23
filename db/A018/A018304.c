/*
 * A018304  Divisors of 148.
 * https://oeis.org/A018304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 148, namely (1, 2, 4, 37, 74, 148)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* list of divisors of 148 in increasing order */
    static const seq_t divisors[6] =
    {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)37,
        (seq_t)74,
        (seq_t)148
    };

    if (n == 0)
        return (seq_t)0;              /* not defined for n=0, return 0 */

    if (n > 6)
        return (seq_t)0;              /* beyond known divisors, return 0 */

    return divisors[n - 1];
}
