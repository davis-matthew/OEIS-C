/*
 * A126675  Product_{i=2..n} |Stirling_1(i,2)|.
 * https://oeis.org/A126675
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=2..n} |Stirling_1(i,2)|
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* If the offset is not respected, the value is irrelevant. */
    if (n < 2) return 1;

    seq_t prod = 1;
    seq_t fact = 1;   /* k!  (initially 1! for k = 1) */
    seq_t c    = 1;   /* c_k = k! * H_k (initially 1 for k = 1) */

    for (idx_t k = 1; k < n; ++k) {
        prod *= c;                     /* multiply current term */

        /* Prepare values for the next k (unless we are done) */
        if (k < n - 1) {
            seq_t newfact = fact * (seq_t)(k + 1);
            seq_t newc    = c * (seq_t)(k + 1) + fact;
            fact = newfact;
            c = newc;
        }
    }
    return prod;
}
