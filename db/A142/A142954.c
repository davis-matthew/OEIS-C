/*
 * A142954  a(n) = 2*n + 3 + 3*(-1)^n.
 * https://oeis.org/A142954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 3 + 3*(-1)^n
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2*n once – the result fits in seq_t for all n that fit in the build */
    seq_t two_n = (seq_t)n * 2;

    /* (-1)^n is -1 when n is odd, +1 when n is even.
       For odd n the formula reduces to 2*n; for even n to 2*n+6. */
    if (n & 1u)
        return two_n;          /* odd n: a(n) = 2*n */
    else
        return two_n + (seq_t)6; /* even n: a(n) = 2*n + 6 */
}
