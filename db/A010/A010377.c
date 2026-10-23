/*
 * A010377  Squares mod 14.
 * https://oeis.org/A010377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 14 * ((n-1)/8) + R[ (n-1)%8 ], where R =
 *            {0,1,2,4,7,8,9,11}
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t R[8] = {0, 1, 2, 4, 7, 8, 9, 11};
    idx_t k = n - 1;           /* assume n >= 1 */
    idx_t block = k / 8;
    idx_t rem   = k % 8;
    seq_t result = (seq_t)14 * (seq_t)block + R[rem];
    return result;
}
