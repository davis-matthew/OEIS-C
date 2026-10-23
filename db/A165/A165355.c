/*
 * A165355  a(n) = 3n + 1 if n is even, or a(n) = (3n + 1)/2 if n is odd.
 * https://oeis.org/A165355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 1 if n is even, otherwise (3*n + 1)/2 if n is odd
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 3 + 1;
    if (n & 1)
        return val / 2;
    else
        return val;
}
