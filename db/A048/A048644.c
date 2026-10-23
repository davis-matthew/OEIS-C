/*
 * A048644  Differences between partial sums of Gray code (A048641) and
 *          triangular numbers (A000217).
 * https://oeis.org/A048644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{j=0..n} (j XOR (j>>1)) - n*(n+1)/2
 * Range:     u8 n <= 46, u16 n <= 766, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t gray = ((seq_t)j) ^ ((seq_t)(j >> 1));
        sum += gray;
    }
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    return sum - tri;
}
