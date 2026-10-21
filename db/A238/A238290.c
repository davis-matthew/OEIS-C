/*
 * A238290  a(n+1) = a(n) + 6 + 2*(n - 2*floor(n/2)) for n > 0, a(0) = 0.
 * https://oeis.org/A238290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n + (n & 1)
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = (14*n - (-1)^n + 1)/2  */

seq_t a(idx_t n)
{
    /* Compute 7*n as seq_t to avoid overflow of idx_t              */
    seq_t result = (seq_t)7 * (seq_t)n;
    /* Add 1 for odd n, 0 for even n                                 */
    result += (seq_t)(n & 1);
    return result;
}
