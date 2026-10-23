/*
 * A100154  Structured truncated icosahedral numbers.
 * https://oeis.org/A100154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/2)*n*(35*n^2 - 47*n + 14)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 588, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute (35*n^2 - 47*n + 14) first to keep intermediates small. */
    seq_t temp = (seq_t)35 * nn * nn - (seq_t)47 * nn + (seq_t)14;
    return nn * temp / (seq_t)2;
}
