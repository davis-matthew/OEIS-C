/*
 * A157810  Period 4: repeat [2, 1, 3, 2].
 * https://oeis.org/A157810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) for n > 4, with a(1) = 2, a(2) = 1, a(3) = 3, a(4)
 *            = 2 (period 4)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[4] = {2, 1, 3, 2};
    return pattern[(n - 1) & 3];
}
