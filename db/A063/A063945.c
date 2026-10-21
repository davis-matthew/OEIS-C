/*
 * A063945  Number of nonnegative integers with n digits.
 * https://oeis.org/A063945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 10; a(n) = 9 * 10^(n-1) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; n=0 is undefined but return 0 to avoid UB */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)10;

    seq_t result = (seq_t)9;            /* 9 * 10^(n-1) */
    for (idx_t i = 1; i < n; ++i)       /* multiply by 10, n-1 times */
        result *= (seq_t)10;
    return result;
}
