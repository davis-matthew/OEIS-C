/*
 * A059029  a(n) = n if n is even, 2*n + 1 if n is odd.
 * https://oeis.org/A059029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is even, 2*n + 1 if n is odd
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is of type idx_t (uint64_t).  Cast to seq_t then compute. */
    if ((n & 1) == 0)
    {
        return (seq_t)n;
    }
    else
    {
        return (seq_t)n * 2 + 1;
    }
}
