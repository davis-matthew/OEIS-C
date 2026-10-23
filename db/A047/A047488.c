/*
 * A047488  Numbers that are congruent to {0, 2, 3, 5, 7} mod 8.
 * https://oeis.org/A047488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + offs[(n-1)%5] where offs = {0,2,3,5,7}
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    static const seq_t offs[5] = {0, 2, 3, 5, 7};
    return (seq_t)q * 8 + offs[r];
}
