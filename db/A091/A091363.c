/*
 * A091363  a(n) = n!*n^3.
 * https://oeis.org/A091363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*n^3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n! as a product 1*2*…*n in the sequence type. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Multiply by n³. */
    seq_t nseq = (seq_t)n;
    fact *= nseq;
    fact *= nseq;
    fact *= nseq;

    return fact;
}
