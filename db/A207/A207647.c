/*
 * A207647  a(n) = Product_{k=1..n} floor((2*n+1)/k - 1).
 * https://oeis.org/A207647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1}^{n} floor((2*n+1)/k - 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n = 0 we return the empty product, which is 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 2*n + 1 once to avoid repeated calculation. */
    seq_t offset = (seq_t)2 * (seq_t)n + (seq_t)1;
    seq_t prod   = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t numerator = offset - (seq_t)k;   /* 2*n + 1 - k */
        seq_t term      = numerator / (seq_t)k; /* floor((2*n+1)/k - 1) */
        prod *= term;                           /* multiply into product */
    }

    return prod;
}
