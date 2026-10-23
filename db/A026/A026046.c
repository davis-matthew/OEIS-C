/*
 * A026046  a(n) = dot_product(1,2,...,n)*(6,7,...,n,1,2,3,4,5).
 * https://oeis.org/A026046
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - 6*n + 38)/3
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast index to seq_t to avoid narrowing */
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn * nn - (seq_t)6 * nn + (seq_t)38);
    return val / (seq_t)3;
}
