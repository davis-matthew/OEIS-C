/*
 * A047351  Numbers that are congruent to {0, 1, 2, 4} mod 7.
 * https://oeis.org/A047351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * ((n-1)/4) + [0,1,2,4][(n-1)%4]
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers congruent to 0,1,2,4 mod 7; offset 1 (a(1)=0).   */
seq_t a(idx_t n)
{
    idx_t block = (n - 1) / 4;
    idx_t r     = (n - 1) % 4;
    static const int offs[4] = {0, 1, 2, 4};
    return (seq_t)block * 7 + offs[r];
}
