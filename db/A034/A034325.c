/*
 * A034325  a(n) is the n-th quintic factorial number divided by 5.
 * https://oeis.org/A034325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^(n-1) * n! * Recurrence: a(1) = 1, a(n) = 5 * n *
 *            a(n-1) (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1, but guard for safety */

    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        result *= (seq_t)5 * (seq_t)i;
    }
    return result;
}
