/*
 * A168460  a(n) = 6 + 10*floor((n-1)/2).
 * https://oeis.org/A168460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 + 10*floor((n-1)/2)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t half = (n - 1) / 2;
    return (seq_t)6 + (seq_t)10 * (seq_t)half;
}
