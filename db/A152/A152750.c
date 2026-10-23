/*
 * A152750  Eight times hexagonal numbers: a(n) = 8*n*(2*n-1).
 * https://oeis.org/A152750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n*(2*n-1)
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t before arithmetic */
    seq_t tn = (seq_t)n;
    /* Compute a(n) = 8 * n * (2*n - 1) safely */
    seq_t term = 8 * tn * ((seq_t)2 * tn - (seq_t)1);
    return term;
}
