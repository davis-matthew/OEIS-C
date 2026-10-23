/*
 * A030511  Graham-Sloane-type lower bound on the size of a ternary (n,3,3)
 *          constant-weight code.
 * https://oeis.org/A030511
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( 2*(n-1)^2 / 3 ) for n >= 3
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* offset starts at n = 3 */
    seq_t m = (seq_t)(n - 1);          /* m = n-1 */
    return (seq_t)2 * m * m / (seq_t)3;/* integer division truncates toward 0 */
}
