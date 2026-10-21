/*
 * A235089  a(n)*Pi is the total length of irregular spiral (center points: 2,
 *          1, 3, 4) after n rotations.
 * https://oeis.org/A235089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - ((n%2==1) ? 2 : 0)
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = ((seq_t)n) * 5;
    val -= ((seq_t)(n & 1)) * 2;  /* subtract 2 if n is odd */
    return val;
}
