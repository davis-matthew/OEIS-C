/*
 * A011877  a(n) = floor(n*(n-1)/24).
 * https://oeis.org/A011877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/24)
 * Range:     u8 n <= 78, u16 n <= 1254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the result is 0, avoiding unsigned underflow. */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;
    return nn * (nn - 1) / 24;
}
