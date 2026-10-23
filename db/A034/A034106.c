/*
 * A034106  Fractional part of square root of n starts with 0: first term of
 *          runs (squares excluded).
 * https://oeis.org/A034106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n+4)^2 + 1
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn + (seq_t)4;
    return t * t + (seq_t)1;
}
