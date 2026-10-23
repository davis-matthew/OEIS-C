/*
 * A173501  a(1) = 2, a(2) = 3, a(n) = 6^(2^(n-3)) for n >= 3.
 * https://oeis.org/A173501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(2^(n-3)) for n >= 3, a(1) = 2, a(2) = 3, a(3) = 6
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;

    /* For n >= 4, a(n) = (a(n-1))^2.  Start with a(3)=6 and square n-3 times. */
    seq_t val = (seq_t)6;
    for (idx_t i = 0; i < n - 3; ++i)
    {
        val = val * val;
    }
    return val;
}
