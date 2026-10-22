/*
 * A006108  Gaussian binomial coefficient [ 2n,n ] for q=4.
 * https://oeis.org/A006108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (4^(n+i) - 1)/(4^i - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t num = ((seq_t)1 << (2 * (n + i))) - 1;
        seq_t den = ((seq_t)1 << (2 * i)) - 1;
        /* after step i, result = [n+i, i]_4, always an integer */
        result = result * num / den;
    }
    return result;
}
