/*
 * A173766  a(n) = (10^n + 11)/3.
 * https://oeis.org/A173766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 11)/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)7;   /* a(1) */
    for (idx_t i = 1; i < n; ++i)
    {
        val = val * (seq_t)10 - (seq_t)33;
    }
    return val;
}
