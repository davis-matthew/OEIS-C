/*
 * A013734  a(n) = 4^(3*n+1).
 * https://oeis.org/A013734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*a(n-1), a(0) = 4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)4;
    for (idx_t i = 0; i < n; ++i)
        val *= (seq_t)64;
    return val;
}
