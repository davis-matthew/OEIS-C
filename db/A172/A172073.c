/*
 * A172073  a(n) = (4*n^3 + n^2 - 3*n)/2.
 * https://oeis.org/A172073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n-3)/2
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* Compute n*(n+1)/2 first to keep intermediate small */
    seq_t half = ((seq_t)n * ((seq_t)n + 1)) / 2;
    /* For n >= 1, (4*n-3) is positive and fits in seq_t */
    seq_t factor = ((seq_t)n * 4) - 3;
    return half * factor;
}
