/*
 * A131385  Product ceiling(n/1)*ceiling(n/2)*ceiling(n/3)*...*ceiling(n/n)
 *          (the 'ceiling factorial').
 * https://oeis.org/A131385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} ceil(n/k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        prod *= ((seq_t)n + (seq_t)k - 1) / (seq_t)k;
    }
    return prod;
}
