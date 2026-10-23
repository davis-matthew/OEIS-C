/*
 * A191903  Number of compositions of odd natural numbers into 4 parts <= n.
 * https://oeis.org/A191903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^4 - (1 + (-1)^n)/2)/2
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 302, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t before any arithmetic to avoid mixing types */
    seq_t m  = (seq_t)n + 1;   /* m = n + 1 */
    seq_t m2 = m * m;         /* m^2 */
    seq_t m4 = m2 * m2;       /* m^4 = (n+1)^4 */

    /* subtract 1 if n is even, since (1 + (-1)^n)/2 = 1 for even n, 0 for odd n */
    if ((n & 1U) == 0)
        m4 -= 1;

    /* divide by 2 to finish the formula */
    return m4 / 2;
}
