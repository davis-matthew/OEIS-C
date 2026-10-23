/*
 * A173314  a(n) = 6*n! + 1.
 * https://oeis.org/A173314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n! + 1 , equivalently a(n) = n*a(n-1) - n + 1 with
 *            a(0)=7
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)7;
    for (idx_t i = 1; i <= n; ++i)
    {
        value = (seq_t)i * (value - (seq_t)1) + (seq_t)1;
    }
    return value;
}
