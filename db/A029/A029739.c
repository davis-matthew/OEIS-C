/*
 * A029739  Numbers that are congruent to {1, 3, 4} mod 6.
 * https://oeis.org/A029739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * ((n-1)/3) + [1,3,4][(n-1)%3]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* values for n < 1 are undefined */
    const seq_t offs[3] = {1, 3, 4};
    idx_t k = n - 1;
    idx_t r = k % 3;
    seq_t block = (seq_t)(k / 3);
    return 6 * block + offs[r];
}
