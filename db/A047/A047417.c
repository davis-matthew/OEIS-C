/*
 * A047417  Numbers that are congruent to {2, 3, 4, 6} mod 8.
 * https://oeis.org/A047417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=8*((n-1)/4)+[2,3,4,6][(n-1)%4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t q = m / 4;
    idx_t r = m % 4;
    const seq_t offs[4] = {2, 3, 4, 6};
    return (seq_t)(8 * q + offs[r]);
}
