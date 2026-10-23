/*
 * A174595  a(n) = 5*n^2/8 - n + 1/2 + (-1)^n*(-3*n^2/8 + n - 1/2).
 * https://oeis.org/A174595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/2)^2 if n even, 4*((n-1)/2)^2 if n odd
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                  /* even n */
        seq_t k = ((seq_t)n) >> 1;
        return k * k;
    } else {                              /* odd n */
        seq_t k = (((seq_t)n) - 1) >> 1;
        return (k * k) * 4;
    }
}
