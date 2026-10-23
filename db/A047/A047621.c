/*
 * A047621  Numbers that are congruent to {3, 5} mod 8.
 * https://oeis.org/A047621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 2 - (-1)^n Equivalent: if n is odd, a(n) = 4*n - 1;
 *            if n is even, a(n) = 4*n - 3
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4*n first */
    seq_t res = (seq_t)n * ((seq_t)4);

    /* Adjust according to parity of n */
    if (n & (idx_t)1)   /* odd n */
        res -= (seq_t)1;
    else                /* even n */
        res -= (seq_t)3;

    return res;
}
