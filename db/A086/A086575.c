/*
 * A086575  a(n) = 4*(10^n - 1).
 * https://oeis.org/A086575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * (10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* a(0) = 0 */

    seq_t pow10 = 1;             /* will hold 10^n */
    for (idx_t i = 0; i < n; ++i)
        pow10 *= 10;             /* multiply by 10 n times */

    return (pow10 - 1) * 4;     /* 4 * (10^n - 1) */
}
