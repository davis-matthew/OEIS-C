/*
 * A199488  a(n) = 10*7^n+1.
 * https://oeis.org/A199488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*7^n + 1. Recurrence: a(0) = 11, a(n) = 7*a(n-1) - 6
 *            for n > 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 11;          /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * 7 - 6;   /* a(i+1) = 7*a(i) - 6 */
    }
    return val;
}
