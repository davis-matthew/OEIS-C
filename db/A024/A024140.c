/*
 * A024140  a(n) = 12^n - 1.
 * https://oeis.org/A024140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 11, with a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;              /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        result = result * 12 + 11; /* recurrence step */
    }
    return result;
}
