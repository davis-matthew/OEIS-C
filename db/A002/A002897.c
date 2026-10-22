/*
 * A002897  a(n) = binomial(2n,n)^3.
 * https://oeis.org/A002897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n,n)^3, binomial(2n,n) = Product_{i=1..n}
 *            (n+i)/i
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        /* b holds binomial(n+i-1, i-1); next value is exact */
        b = b * ((seq_t)n + (seq_t)i) / (seq_t)i;
    }
    return b * b * b;
}
