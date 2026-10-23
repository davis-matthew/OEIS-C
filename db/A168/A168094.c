/*
 * A168094  a(n) = number of natural numbers m such that n - 4 <= m <= n + 4.
 * https://oeis.org/A168094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n + 4 for 0 <= n <= 4, a(n) = 9 for n >= 5
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n is idx_t (unsigned), the comparison is safe. */
    if (n <= 4)
        return (seq_t)(n + 4);
    else
        return (seq_t)9;
}
