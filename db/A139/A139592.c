/*
 * A139592  A033585(n) followed by A139271(n+1).
 * https://oeis.org/A139592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*floor((n + 1/4)^2)
 * Range:     u8 n <= 3, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = 4*n + 1 */
    seq_t t = (seq_t)n * 4 + 1;
    /* t^2 */
    seq_t t2 = t * t;
    /* floor(t^2 / 16) * 2 */
    return (t2 / 16) * 2;
}
