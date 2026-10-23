/*
 * A024052  a(n) = 5^n - n^3.
 * https://oeis.org/A024052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - n^3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow5 *= (seq_t)5;
    seq_t ns = (seq_t)n;
    seq_t n3 = ns * ns * ns;
    return pow5 - n3;
}
