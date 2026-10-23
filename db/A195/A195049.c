/*
 * A195049  Concentric 21-gonal numbers.
 * https://oeis.org/A195049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*n^2/4 + 17*(((-1)^n)-1)/8
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the equivalent integer form
   a(n) = (42*n^2 - 34*(n mod 2)) / 8, which is exact for all n. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute 42 * n * n; the result fits in seq_t for all expected n */
    seq_t num = 42 * nn * nn;

    /* When n is odd subtract 34 to account for the (-1)^n term */
    if (n & 1U)
        num -= (seq_t)34;

    /* divide by 8 to finish the formula */
    return num / (seq_t)8;
}
