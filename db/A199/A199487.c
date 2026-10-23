/*
 * A199487  a(n) = 9*7^n+1.
 * https://oeis.org/A199487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*7^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 10, a(n) = 7*a(n-1) - 6 for n >= 1 */
seq_t a(idx_t n)
{
    /* 9*7^0 = 9, then each loop multiplies by 7 */
    seq_t res = (seq_t)9;
    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (seq_t)7;
    }
    return res + (seq_t)1;
}
