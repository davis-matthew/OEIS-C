/*
 * A047467  Numbers that are congruent to {0, 2} mod 8.
 * https://oeis.org/A047467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 5 - (-1)^n (OEIS A047467)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to sequence type for arithmetic */
    seq_t nn = (seq_t)n;
    /* Compute 4*n */
    seq_t val = (seq_t)4 * nn;
    /* Subtract 4 for odd n, 6 for even n */
    seq_t sub = (n & 1ULL) ? (seq_t)4 : (seq_t)6;
    return val - sub;
}
