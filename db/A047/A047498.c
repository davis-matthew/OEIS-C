/*
 * A047498  Numbers that are congruent to {0, 1, 2, 4, 5, 7} mod 8.
 * https://oeis.org/A047498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/6) + pattern[(n-1)%6] where pattern =
 *            {0,1,2,4,5,7}
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {0, 1, 2, 4, 5, 7};
    idx_t k = n - 1;
    idx_t q = k / 6;
    idx_t r = k % 6;
    return (seq_t)8 * (seq_t)q + pattern[r];
}
