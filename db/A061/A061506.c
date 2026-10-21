/*
 * A061506  a(n) = lcm(6n+2, 6n+4, 6n+6).
 * https://oeis.org/A061506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*(3*n+1)*(3*n+2)*(n+1) if n is even * a(n) =
 *            3*(3*n+1)*(3*n+2)*(n+1) if n is odd * This follows directly from
 *            the OEIS entry for A061506
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 429, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the sequence type once */
    seq_t N = (seq_t)n;
    /* compute the common factors */
    seq_t x1 = (N * (seq_t)3) + (seq_t)1;   /* 3*n + 1 */
    seq_t x2 = (N * (seq_t)3) + (seq_t)2;   /* 3*n + 2 */
    seq_t x3 = N + (seq_t)1;                /* n + 1 */

    /* multiply the three factors */
    seq_t prod = x1 * x2;
    prod = prod * x3;

    /* apply the parity multiplier */
    if (n & (idx_t)1) {
        prod = prod * (seq_t)3;   /* odd n */
    } else {
        prod = prod * (seq_t)6;   /* even n */
    }

    return prod;
}
