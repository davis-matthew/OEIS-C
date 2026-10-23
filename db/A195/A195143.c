/*
 * A195143  a(n) = n-th concentric 12-gonal number.
 * https://oeis.org/A195143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + (-1)^n - 1
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to the sequence type */
    seq_t res = 3 * nn * nn;      /* base term 3*n^2 */
    if (n & 1)                    /* if n is odd: subtract 2 */
        res -= (seq_t)2;
    /* for even n no adjustment is needed (3*n^2 + 1 - 1 = 3*n^2) */
    return res;
}
