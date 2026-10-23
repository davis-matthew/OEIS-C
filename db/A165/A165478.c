/*
 * A165478  Positions of zeros in A165477.
 * https://oeis.org/A165478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)*131071 + (n%2)*131070
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = (seq_t)(n / 2);      /* floor(n/2) */
    seq_t odd  = (seq_t)(n & 1);      /* n mod 2 */

    return half * 131071 + odd * 131070;
}
