/*
 * A168095  a(n) = number of natural numbers m such that n - 5 <= m <= n + 5.
 * https://oeis.org/A168095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n + 5 for 0 <= n <= 5, a(n) = 11 for n >= 6
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6)
        return (seq_t)(n + 5);
    else
        return (seq_t)11;
}
