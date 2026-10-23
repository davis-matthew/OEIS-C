/*
 * A195463  a(n) = 4^(n+1) + 7.
 * https://oeis.org/A195463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n+1) + 7
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    /* Multiply 4, n+1 times */
    for (idx_t i = 0; i < n + 1; ++i)
    {
        res *= (seq_t)4;
    }
    /* Add the constant term 7 */
    res += (seq_t)7;
    return res;
}
