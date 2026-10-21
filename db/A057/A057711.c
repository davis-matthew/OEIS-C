/*
 * A057711  a(0)=0, a(1)=1, a(n) = n*2^(n-2) for n >= 2.
 * https://oeis.org/A057711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 2^(n-2) for n >= 2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t val = n;               /* start with n */
    for (idx_t i = 0; i < n - 2; ++i)
        val = val + val;         /* multiply by 2 via addition   */
    return val;
}
