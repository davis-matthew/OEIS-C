/*
 * A063495  a(n) = (2*n-1)*(5*n^2-5*n+2)/2.
 * https://oeis.org/A063495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(5*n^2-5*n+2)/2
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 778, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert index to sequence type
    seq_t nseq = (seq_t)n;

    // Compute the two factors
    seq_t term1 = (seq_t)2 * nseq - (seq_t)1;            // 2*n - 1
    seq_t term2 = (seq_t)5 * nseq * nseq - (seq_t)5 * nseq + (seq_t)2;  // 5*n^2 - 5*n + 2

    // Product divided by 2 (integer division is exact)
    seq_t prod = term1 * term2;
    return prod / (seq_t)2;
}
