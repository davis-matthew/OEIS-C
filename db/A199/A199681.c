/*
 * A199681  a(n) = 11*9^n+1.
 * https://oeis.org/A199681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*9^n + 1 * Recurrence: a(0) = 12; a(n) = 9*a(n-1) - 8
 *            for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)12;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)9 - (seq_t)8;
    }
    return val;
}
