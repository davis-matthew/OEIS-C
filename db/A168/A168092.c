/*
 * A168092  a(n) = number of natural numbers m such that n - 2 <= m <= n + 2.
 * https://oeis.org/A168092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n+2 for 0 <= n <= 2, a(n) = 5 for n >= 3
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)n + (seq_t)2;
    else
        return (seq_t)5;
}
