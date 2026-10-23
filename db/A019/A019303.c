/*
 * A019303  "Pascal sweep" for k=2: draw a horizontal line through the 1 at
 *          C(k,0) in Pascal's triangle; rotate this line and record the sum
 *          of the numbers on it (excluding the initial 1).
 * https://oeis.org/A019303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 1, a(2n+1) = n+4
 * Range:     u8 n <= 504, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return 1;
    } else {
        /* compute (n-1)/2 in seq_t then add 4 */
        return ((seq_t)(n - 1)) / 2 + 4;
    }
}
