/*
 * A047456  Numbers that are congruent to {0, 2, 3, 4} mod 8.
 * https://oeis.org/A047456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,2,3,4][(n-1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char offs[4] = {0, 2, 3, 4};
    idx_t k = (n - 1) / 4;          /* block number */
    unsigned int m = (unsigned int)(n - 1) % 4;  /* position within block */
    return (seq_t)8 * (seq_t)k + offs[m];
}
