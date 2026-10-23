/*
 * A011656  A binary m-sequence: expansion of reciprocal of x^3 + x^2 + 1 (mod
 *          2), shifted by 2 initial 0's.
 * https://oeis.org/A011656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[n mod 7] where pattern = {0, 0, 1, 0, 1, 1, 1}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[7] = {0, 0, 1, 0, 1, 1, 1};
    idx_t rem = n % (idx_t)7;
    return pattern[rem];
}
