/*
 * A054577  A Catalan-like sequence.
 * https://oeis.org/A054577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( C(2n, n) / (n+2) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(2n, n) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Multiply by (n + i) and divide by i -- the intermediate result
           stays integral, so truncating division is exact.  */
        binom = binom * (seq_t)(n + i);
        binom = binom / (seq_t)i;
    }

    /* Divide by (n + 2) and floor the result */
    seq_t denom = (seq_t)(n + 2);
    return binom / denom;
}
